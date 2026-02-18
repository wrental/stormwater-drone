#ifndef STORMWATER_SPOOL_H
#define STORMWATER_SPOOL_H

#include "driver/gpio.h"
/*!
 * @brief initialize spool (must be coiled)
 */
void stormwater_spool_init(void);

/*!
 * @brief returns spool position in inches
 */
float stormwater_spool_get_pos(void);

/*!
 * @brief set spool to 0 (up) or 1 (down)
 */
void stormwater_spool_set(uint8_t opt);

#endif
