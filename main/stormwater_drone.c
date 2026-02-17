/* name: stormwater_drone.c
 * date: 02-11-2026
 * auth: wrental
 * desc: why am i bothering with this again
 */

// stdlib components
#include <stdio.h>

// project components
#include "stormwater_drone.h"
#include "stormwater_lora.h"

// esp-idf components
#include "freertos/task.h"

/*
 * @brief main app - call init functions, start loop
 */
void app_main(void) {
  stormwater_lora_init();

}
