/* name: setup_stormwater_drone.h
 * date: 02-11-2026
 * auth: wrental
 * desc: setup function headers for drone
 */

#ifndef SETUP_STORMWATER_DRONE_H
#define SETUP_STORMWATER_DRONE_H

#include "esp_lora_1121.h"

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "lr1121_config.h"
#include "lr11xx_printf_info.h"
#include "lr11xx_system_types.h"

esp_err_t spi_bus_init();
void lr1121_test_task(void * arg);
void test_app();

#endif
