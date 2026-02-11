/* name: v2-stormwater-drone.c
 * date: 02-10-2026
 * auth: wrental
 * desc: version way more than 2 ngl
 */

#include <stdio.h>

#include "esp_lora_1121.h"

#include "lr1121_config.h"
#include "setup_stormwater_drone.h"

void app_main(void) {
  // program setup
  const char * version = lr11xx_driver_version_get_version_string();
  printf("INIT: lr11xx driver version: %s\n", version);
  test_app();
}


