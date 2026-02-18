#ifndef STORMWATER_PUMP_H
#define STORMWATER_PUMP_H

#include "driver/gpio.h"

#define ESP_PUMP 		(GPIO_NUM_1)

/*!
 * @brief initialize pump gpio - requires MOSFET circuit!
 */
void stormwater_pump_init(void);

/*!
 * @brief set pump 1 for ON or 0 for OFF
 */
void stormwater_pump_set(uint8_t opt);

#endif
