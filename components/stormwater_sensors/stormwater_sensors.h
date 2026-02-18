#ifndef STORMWATER_SENSORS_H
#define STORMWATER_SENSORS_H

// all necessary includes for the stormwater sensors component
// besides onewire, all three libraries are from the ESP-IDF framework
#include <stdint.h>
#include "onewire.h"
#include "esp_adc/adc_oneshot.h"
#include "driver/gpio.h"
#include "esp_log.h"

// TODO: Update these pins once hardware is finalized
#define PH_ADC_CHANNEL      ADC_CHANNEL_0   // placeholder
#define DO_ADC_CHANNEL      ADC_CHANNEL_1   // placeholder
#define TEMP_GPIO           GPIO_NUM_4      // placeholder

// Calibration constants for pH sensor
#define PH_OFFSET       -17.125f
#define PH_GAIN         12.5f

// Sampling and printing intervals
#define SAMPLING_INTERVAL_MS  20
#define PRINT_INTERVAL_MS     800
#define ARRAY_LENGTH    40

// ADC reference voltage and resolution
#define VREF_MV         5000
#define ADC_RES         4095

// DO2 calibration constants
#define CAL1_V          1601
#define CAL1_T          36.25f
#define CAL2_V          1132
#define CAL2_T          17.94f

// Function declarations 
void sensors_init(void);
void sensors_task(void *pvParameters);
float get_temp(void);
float read_do(uint32_t voltage_mv, uint8_t temperature_c);
double average_array(int *arr, int number);

#endif

