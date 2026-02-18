/* name: stormwater_drone.c
 * date: 02-11-2026
 * auth: wrental
 * desc: why am i bothering with this again
 */

// stdlib components
#include <stdio.h>

// project components
#include "stormwater_drone.h"
#include "stormwater_drone_lora.h"
#include "stormwater_pump.h"
#include "stormwater_sensors.h"

// esp-idf components

// predefined memory allocation
uint8_t temp;
uint8_t do_2;
uint8_t pH;

/*
 * @brief main app - call init functions, start loop
 */
void app_main(void) {
  sensors_init();
  stormwater_pump_init();
  stormwater_drone_lora_init();



  while(1) {

    temp = (uint8_t) get_temp();
    do_2 = (uint8_t) read_do(3300, temp);
    pH = (uint8_t) read_pH();

    memcpy (&temp, stormwater_drone_lora_send_packet, 1);
    memcpy (&do_2, stormwater_drone_lora_send_packet + 1, 1);
    memcpy (&pH, stormwater_drone_lora_send_packet + 2, 1);

    if(stormwater_drone_lora_irq_flag) {
      stormwater_drone_lora_irq_process();
      printf("%i\n", *stormwater_drone_lora_receive_packet);
    }
  }
    

  // Start the sensors task? (idk chat gpt said to do this in the main function)
  // xTaskCreate(sensors_task, "sensors_task", 4096, NULL, 5, NULL); 

}
