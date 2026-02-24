#ifndef STORMWATER_SPOOL_H
#define STORMWATER_SPOOL_H

#include "driver/gpio.h"
/*!
 * @brief initialize spool (must be coiled)
 */
void stormwater_spool_init(void);

/*!
 * @brief set spool to 0 (up) or 1 (down)
 *
 * TODO: add increments (?)
 */
void stormwater_spool_set(uint8_t opt);

#endif
