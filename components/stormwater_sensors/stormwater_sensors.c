/*
  auth: Colby Robbins
  date: 02-17-2026
  desc: 
  This is combined sensor control code that allows for simultaneous temperature, dissolved oxygen, and pH readings.
  pH readings are averaged over 40 samples to reduce noise, and the DO sensor is calibrated using a lookup table based on temperature.
  The code is structured to run as a FreeRTOS task, with non-blocking timing. 
  Translated from Arduino C++ to C for use in the ESP-IDF framework.
*/
#include "stormwater_sensors.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Tag for logging
static const char *TAG = "StormwaterSensors";

// ADC (analog to digital converter) handle for pH and DO sensors
static adc_oneshot_unit_handle_t adc_handle = NULL;

// pH averaging array and index
static int pH_array[ARRAY_LENGTH];
static int pH_array_index=0;

// Temperature reading
static float temperature_c = 0.0f;

// DO saturation table based on temperature (0-40°C)
static const uint16_t DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};

void sensors_init(void)
{
  // Initialize and configure ADC channels for pH and DO sensors
  adc_oneshot_unit_init_cfg_t init_config = {
    .unit_id = ADC_UNIT_1,
  };
  adc_oneshot_new_unit(&init_config, &adc_handle);

  adc_oneshot_chan_cfg_t channel_config = {
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_12,
  };

  adc_oneshot_config_channel(adc_handle, PH_ADC_CHANNEL, &channel_config);
  adc_oneshot_config_channel(adc_handle, DO_ADC_CHANNEL, &channel_config);

  ESP_LOGI(TAG, "ADC initialized for pH and DO sensors");
}

void sensors_task(void *pvParameters)
{
  static int64_t samplingTime = 0;
  static int64_t printTime = 0;
  static float pHValue, voltage;

  if(samplingTime == 0) samplingTime = esp_timer_get_time() / 1000;
  if(printTime == 0) printTime = esp_timer_get_time() / 1000;
  
  while(1)
  {
    // Sample pH every 20 milliseconds
    if((esp_timer_get_time() / 1000) - samplingTime > SAMPLING_INTERVAL_MS)
    {
      // Read raw ADC value for pH sensor and store in array
      int raw = 0;
      adc_oneshot_read(adc_handle, PH_ADC_CHANNEL, &raw);
      pH_array[pH_array_index++] = raw;

      // If array is full, reset index and calculate average pH value
      if(pH_array_index==ARRAY_LENGTH)pH_array_index=0;
      voltage = average_array(pH_array, ARRAY_LENGTH)*3.3f/ADC_RES;
      pHValue = PH_GAIN*voltage+PH_OFFSET;
      samplingTime=esp_timer_get_time() / 1000;
    }

    // Print values every 800 milliseconds
    if((esp_timer_get_time() / 1000) - printTime > PRINT_INTERVAL_MS)
    {
      // Read temperature and dissolved oxygen values
      temperature_c = get_temp();
      float temperature_f = (temperature_c* 1.8) + 32;
      int do_raw = 0;
      adc_oneshot_read(adc_handle, DO_ADC_CHANNEL, &do_raw);
      uint16_t do_voltage_mv = (uint32_t)VREF_MV * do_raw / ADC_RES;

      // Log all sensor readings together
      ESP_LOGI(TAG, "Temp: %.2f°F  |  pH: %.2f  |  pH Voltage: %.2f V |  DO: %.2f µg/L",
         temperature_f, pHValue, voltage, read_do(do_voltage_mv, (uint8_t)temperature_c));
    
      printTime=esp_timer_get_time() / 1000;
    }
    // Short delay to prevents tasks from hogging CPU
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

double average_array(int* arr, int number){
  // Averages an array of integers, excluding the max and min values if the array has 5 or more elements
  int i;
  int max,min;
  double avg;
  long amount=0;

  if(number<=0){
    ESP_LOGE(TAG, "Invalid array length");
    return 0;
  }

  // less than 5, calculated directly statistics
  if(number<5){
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = (double)amount/number;
    return avg;
  // 5 or more, exclude max and min
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }
    }
    avg = (double)amount/(number-2);
  }
  return avg;
}

float get_temp(){
  // Returns the temperature from its port in DEG Celsius

  uint8_t data[12];

  onewire_reset(TEMP_GPIO);
  onewire_skip_rom(TEMP_GPIO);
  onewire_write(TEMP_GPIO, 0x44); // start conversion, with parasite power on at the end
  vTaskDelay(pdMS_TO_TICKS(750)); // conversion wait
  onewire_reset(TEMP_GPIO);

  onewire_skip_rom(TEMP_GPIO);
  onewire_write(TEMP_GPIO, 0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = (uint8_t)onewire_read(TEMP_GPIO);
  }

  uint8_t MSB = data[1];
  uint8_t LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16.f;

  return TemperatureSum;

}

float read_do(uint32_t voltage_mv, uint8_t temp_c)
{
  uint16_t V_saturation = (int16_t)((float)temp_c - CAL2_T) * (CAL1_V - CAL2_V) / (CAL1_T - CAL2_T) + CAL2_V;
  return (float)(voltage_mv * DO_Table[temp_c]) / V_saturation;
}

float read_pH()
{
  float voltage = 0;
  static int64_t samplingTime = 0;
  if(samplingTime == 0) samplingTime = esp_timer_get_time() / 1000;
  
  while(pH_array_index!=ARRAY_LENGTH){
    if((esp_timer_get_time() / 1000) - samplingTime > SAMPLING_INTERVAL_MS)
    {
      // Read raw ADC value for pH sensor and store in array
      int raw = 0;
      adc_oneshot_read(adc_handle, PH_ADC_CHANNEL, &raw);
      pH_array[pH_array_index++] = raw;
      samplingTime = esp_timer_get_time() / 1000;
    }
  }
  double average = average_array(pH_array, ARRAY_LENGTH);
  if(average == 0){
    ESP_LOGE(TAG, "Average pH reading is zero, check sensor connection");
    return 0;
  }
  // If array is full, calculate average pH value and reset pH index
  voltage = (float)average*3.3f/ADC_RES;
  pH_array_index = 0;
  return PH_GAIN*voltage+PH_OFFSET;
}
