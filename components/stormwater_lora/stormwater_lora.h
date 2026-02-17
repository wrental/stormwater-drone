/*!
 * @file	stormwater_lora.h
 *
 * @brief	common LoRa component of stormwater project, including SPI setup
 *
 */

#ifndef STORMWATER_LORA_H
#define STORMWATER_LORA_H

#include "lr11xx_system_types.h"
#include "stormwater_lr1121_config.h"

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "hal/spi_types.h"

// Waveshare Core1121XF IO
#define CS              (GPIO_NUM_1)
#define CLK             (GPIO_NUM_2)
#define MOSI            (GPIO_NUM_3)
#define MISO            (GPIO_NUM_4)
#define RESET           (GPIO_NUM_5)
#define BUSY            (GPIO_NUM_6)
#define INT							(GPIO_NUM_7)    // interrupt pin >> DIO9

// lr1121 Interrupt Flags
#define IRQ_MASK                                                                          \
    ( LR11XX_SYSTEM_IRQ_TX_DONE | LR11XX_SYSTEM_IRQ_RX_DONE | LR11XX_SYSTEM_IRQ_TIMEOUT | \
      LR11XX_SYSTEM_IRQ_HEADER_ERROR | LR11XX_SYSTEM_IRQ_CRC_ERROR | LR11XX_SYSTEM_IRQ_FSK_LEN_ERROR )

// ESP32-S3 SPI
#define ESP_SPI_NUM             (SPI2_HOST)
#define ESP_SPI_CLK_SPEED_HZ    8 * 1000 * 1000 // 8MHz

void stormwater_lora_init(void);
void stormwater_lora_irq(const void * context, lr11xx_system_irq_mask_t irq_flags);

#endif
