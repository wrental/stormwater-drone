#ifndef V3_STORMWATER_DRONE_H
#define V3_STORMWATER_DRONE_H

// std libs
#include <stdio.h>

// esp-idf common components
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "hal/spi_types.h"
#include "esp_err.h"

// waveshare/esp_lora_1121
#include "esp_lora_1121.h"
#include "lr1121_config.h"

// esp32-s3 config
#define ESP_SPI_CS              (GPIO_NUM_1)
#define ESP_SPI_CLK             (GPIO_NUM_2)
#define ESP_SPI_MOSI            (GPIO_NUM_3)
#define ESP_SPI_MISO            (GPIO_NUM_4)
#define ESP_SPI_RESET           (GPIO_NUM_5)
#define ESP_SPI_BUSY            (GPIO_NUM_6)

#define ESP_SPI_INT             (GPIO_NUM_7)    // interrupt pin >> 1121_DIO9

#define ESP_SPI_NUM             (SPI2_HOST)
#define ESP_SPI_CLK_SPEED_HZ    8 * 1000 * 1000 // 8MHz

#define IRQ_MASK                                                                          \
    ( LR11XX_SYSTEM_IRQ_TX_DONE | LR11XX_SYSTEM_IRQ_RX_DONE | LR11XX_SYSTEM_IRQ_TIMEOUT | \
      LR11XX_SYSTEM_IRQ_HEADER_ERROR | LR11XX_SYSTEM_IRQ_CRC_ERROR | LR11XX_SYSTEM_IRQ_FSK_LEN_ERROR )

static spi_bus_config_t spi_bus_config = {
	.sclk_io_num = ESP_SPI_CLK,
	.mosi_io_num = ESP_SPI_MOSI,
	.miso_io_num = ESP_SPI_MISO,
  .quadwp_io_num = -1,
  .quadhd_io_num = -1,
  .max_transfer_sz = 64,
};

static spi_device_interface_config_t spi_device_interface_config = {
	.clock_speed_hz = ESP_SPI_CLK_SPEED_HZ,		// 8 MHz
  .mode = 0,	// SPI mode 0: CPOL=0, CPHA=0
  .spics_io_num = -1,
  .queue_size = 1,
};

static spi_device_handle_t spi_device_handle = NULL; 	// SPI handle

static bool irq_flag;
static void IRAM_ATTR isr(void* arg) {
    irq_flag = true; // Set the interrupt flag
}

#endif
