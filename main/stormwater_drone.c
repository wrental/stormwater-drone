/* name: stormwater_drone.c
 * date: 02-11-2026
 * auth: wrental
 * desc: why am i bothering with this again
 */

// stdlib components

// project components
#include "stormwater_drone.h"
#include "stormwater_pump.h"
#include "stormwater_sensors.h"

// esp-idf components

/*
 * @brief main app - call init functions, start loop
 */
void app_main(void) {
  sensors_init();
  stormwater_pump_init();

  // Start the sensors task? (idk chat gpt said to do this in the main function)
  // xTaskCreate(sensors_task, "sensors_task", 4096, NULL, 5, NULL); 

}
