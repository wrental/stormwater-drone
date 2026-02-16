/* name: stormwater_drone.c
 * date: 02-11-2026
 * auth: wrental
 * desc: why am i bothering with this again
 */


#include "stormwater_drone.h"
#include "lr1121_config.h"
#include "lr11xx_types.h"
#include "lr11xx_types_str.h"

// helper functions
static bool irq_flag;
static void IRAM_ATTR isr(void* arg) {
  irq_flag = true; // Set the interrupt flag
}

/**
 *  @brief lora init via lora_config.h, esp_lora_1121.h functions 
 */
static void init_lora(void) {
  printf("Initializing LoRa over SPI...\n");
  lora_init_io_context(&lr1121, ESP_SPI_CS, ESP_SPI_RESET, ESP_SPI_BUSY, ESP_SPI_INT);
  lora_init_io(&lr1121);
  lora_spi_init(&lr1121, spi_device_handle);
  lora_system_init(&lr1121);
  lora_print_version(&lr1121);
  lora_radio_init(&lr1121);
  lora_init_irq(&lr1121, isr);
  lr11xx_status_t radio_status = lr11xx_radio_set_rx(&lr1121, RX_CONTINUOUS);
  printf("%s\n", lr11xx_status_to_str(radio_status));
}

/*
 * @brief spi init via driver/spi_*.h components from esp-idf
 */
static void init_spi(void) {
  printf("Initializing SPI2_HOST\n");
  spi_bus_initialize(SPI2_HOST, &spi_bus_config, SPI_DMA_CH_AUTO);
  spi_bus_add_device(SPI2_HOST, &spi_device_interface_config, &spi_device_handle);
}



/*
 * @brief main app - call init functions, start loop
 */
void app_main(void) {
  init_spi();
  init_lora();



    
}
