/* name: v3_stormwater_drone.c
 * date: 02-11-2026
 * auth: wrental
 * desc: why am i bothering with this again
 */


#include "v3_stormwater_drone.h"

// helper functions

static bool irq_flag;
static void IRAM_ATTR isr(void* arg) {
  irq_flag = true; // Set the interrupt flag
}


// --- MAIN ---
void app_main(void) {

  // ESP-32-S3 SPI Initialization

  printf("Initializing SPI2_HOST\n");
  spi_bus_initialize(SPI2_HOST, &spi_bus_config, SPI_DMA_CH_AUTO);
  spi_bus_add_device(SPI2_HOST, &spi_device_interface_config, &spi_device_handle);


  // Waveshare Core1121 HF Initialization

  printf("Initializing LoRa over SPI...\n");
  lora_init_io_context(&lr1121, ESP_SPI_CS, ESP_SPI_RESET, ESP_SPI_BUSY, ESP_SPI_INT);
  lora_init_io(&lr1121);
  lora_spi_init(&lr1121, spi_device_handle);
  lora_system_init(&lr1121);
  lora_print_version(&lr1121);
  lora_radio_init(&lr1121);
  lora_init_irq(&lr1121, isr);
  
}
