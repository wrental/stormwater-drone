/*!
 *  @file 	stormwater_lora.c
 *
 *  @brief 	common LoRa component of stormwater project, including SPI setup
 *
 */

#include "stormwater_lora.h"

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "hal/spi_types.h"

#include "lr11xx_radio.h"
#include "lr11xx_system.h"
#include "lr11xx_system_types.h"

// --- SPI CONFIG ---
static spi_bus_config_t spi_bus_config = {
  .sclk_io_num = CLK,
  .mosi_io_num = MOSI,
  .miso_io_num = MISO,
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


// --- INTERRUPT PROCESS CONFIG ---

static bool irq_flag;

static void IRAM_ATTR isr(void* arg) {
  irq_flag = true; // Set the interrupt flag
}

static void on_rx_done(void) {

}


// --- PUBLIC METHODS ---
/*
 * @brief initialize LoRa module on ESP_SPI_NUM bus
 */
void stormwater_lora_init(void) {
  printf("Initializing SPI2_HOST\n");
  spi_bus_initialize(ESP_SPI_NUM, &spi_bus_config, SPI_DMA_CH_AUTO);
  spi_bus_add_device(ESP_SPI_NUM, &spi_device_interface_config, &spi_device_handle);

  printf("Initializing LoRa over SPI...\n");
  lora_init_io_context(&lr1121, CS, RESET, BUSY, INT);
  lora_init_io(&lr1121);
  lora_spi_init(&lr1121, spi_device_handle);
  lora_system_init(&lr1121);
  lora_print_version(&lr1121);
  lora_radio_init(&lr1121);
  lora_init_irq(&lr1121, isr);
}

/*
 * @brief LoRa module interrupt process
 */
void stormwater_lora_irq(const void* context, lr11xx_system_irq_mask_t irq_filter_mask) {
  
  irq_flag = false;

  lr11xx_system_irq_mask_t irq_regs;
  lr11xx_system_get_and_clear_irq_status(&lr1121, &irq_regs);

  ESP_LOGD(LR1121_TAG, "Interrupt flags = 0x%08lX\n", &irq_regs);

  irq_regs &= irq_filter_mask;

  ESP_LOGD(LR1121_TAG, "Filtered interrupt flags = 0x%08lX\n", &irq_regs);

}

