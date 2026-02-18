#include "stormwater_drone_lora.h"
#include "esp_lora_1121.h"

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "freertos/idf_additions.h"
#include "portmacro.h"

#include "lr1121_config.h"
#include "lr11xx_radio.h"
#include "lr11xx_regmem.h"
#include "lr11xx_system_types.h"


// --- PRIVATE DEFS AND METHODS ---

static spi_device_handle_t stormwater_drone_spi_handle = NULL;

static void IRAM_ATTR isr(void* arg) {
	stormwater_drone_lora_irq_flag = true;
}

static void stormwater_drone_spi_init(void) {
	spi_bus_config_t stormwater_drone_spi_config = {
		.mosi_io_num = ESP_MOSI,
		.miso_io_num = ESP_MISO,
		.sclk_io_num = ESP_CLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 64,
	};

	spi_device_interface_config_t stormwater_drone_spi_device_config = {
		.clock_speed_hz = ESP_SPI_CLK_HZ,
		.mode = 0,
		.spics_io_num = -1,
		.queue_size = -1,
	};


	spi_bus_initialize(ESP_SPI_HOST, &stormwater_drone_spi_config, SPI_DMA_CH_AUTO);
	spi_bus_add_device(ESP_SPI_HOST, &stormwater_drone_spi_device_config, &stormwater_drone_spi_handle);
}

static void reception_failure(void) {
	if(IS_HOST) {
		// TODO: add debug message: client failed to respond
		lr11xx_regmem_write_buffer8(&lr1121, stormwater_drone_lora_send_packet, PAYLOAD_LENGTH);
		lr11xx_radio_set_tx(&lr1121, 0);
	}
	else {
		lr11xx_radio_set_rx(&lr1121, RX_CONTINUOUS);
	}
}


static void on_tx_done(void) {
	lr11xx_radio_set_rx(&lr1121, (2 * get_time_on_air_in_ms() + TX_RX_TRANSITION_DELAY + ITERATION_DELAY));
}

static void lora_receive(const void* context, uint8_t* buffer, uint8_t buffer_length, uint8_t* size) {
	lr11xx_radio_rx_buffer_status_t rx_buffer_status;
	lr11xx_radio_pkt_status_lora_t pkt_status_lora;
	lr11xx_radio_pkt_status_gfsk_t pkt_status_gfsk;

	lr11xx_radio_get_rx_buffer_status(&lr1121, &rx_buffer_status);
	*size = rx_buffer_status.pld_len_in_bytes;
	if(*size > buffer_length) {
		// TODO: add debug - payload larger than buffer
		return;
	}

	lr11xx_regmem_read_buffer8(&lr1121, buffer, rx_buffer_status.buffer_start_pointer, 
			rx_buffer_status.pld_len_in_bytes);
	// TODO: add paket status debug
	
}

static void on_rx_done(void) {
	uint8_t size;
	lora_receive(&lr1121, stormwater_drone_lora_receive_packet, PAYLOAD_LENGTH, &size);
	vTaskDelay(ITERATION_DELAY / portTICK_PERIOD_MS);
	lr11xx_regmem_write_buffer8(&lr1121, stormwater_drone_lora_send_packet, PAYLOAD_LENGTH);
	lr11xx_radio_set_tx(&lr1121, 0);
}

static void on_rx_timeout() {
	// TODO: add debug msg
	reception_failure();
}




// --- PUBLIC METHODS ---

void stormwater_drone_lora_init(void) {
	stormwater_drone_spi_init();

	lora_init_io_context(&lr1121, ESP_CS, ESP_RESET, ESP_BUSY, ESP_INT);
	lora_init_io(&lr1121);

	lora_spi_init(&lr1121, stormwater_drone_spi_handle);

	lora_system_init(&lr1121);
	lora_radio_init(&lr1121);
	lora_init_irq(&lr1121, isr);

	lr11xx_system_set_dio_irq_params( &lr1121, IRQ_MASK, 0 );
	lr11xx_system_clear_irq_status( &lr1121, LR11XX_SYSTEM_IRQ_ALL_MASK );

	lr11xx_regmem_write_buffer8(&lr1121, stormwater_drone_lora_send_packet, PAYLOAD_LENGTH);

	if(IS_HOST) {
		lr11xx_radio_set_tx(&lr1121, 0);
	}
	else {
		lr11xx_radio_set_rx(&lr1121, RX_CONTINUOUS);
	}
}

void stormwater_drone_lora_irq_process(void) {
	stormwater_drone_lora_irq_flag = false;
	lr11xx_system_irq_mask_t irq_regs;
	lr11xx_system_get_and_clear_irq_status(&lr1121, &irq_regs);

	// parse flags
	irq_regs &= IRQ_MASK;

	if((irq_regs & LR11XX_SYSTEM_IRQ_TX_DONE) == LR11XX_SYSTEM_IRQ_TX_DONE) {
		on_tx_done();
	}
	if((irq_regs & LR11XX_SYSTEM_IRQ_HEADER_ERROR) == LR11XX_SYSTEM_IRQ_HEADER_ERROR) {
		// TODO: add debug message
	}
	if((irq_regs & LR11XX_SYSTEM_IRQ_RX_DONE) == LR11XX_SYSTEM_IRQ_RX_DONE) {
		if((irq_regs & LR11XX_SYSTEM_IRQ_CRC_ERROR) == LR11XX_SYSTEM_IRQ_CRC_ERROR) {
			reception_failure();
		}
		else if((irq_regs & LR11XX_SYSTEM_IRQ_FSK_LEN_ERROR) == LR11XX_SYSTEM_IRQ_FSK_LEN_ERROR) {
			reception_failure();
		}
		else {
			on_rx_done();
		}
	}
	if((irq_regs & LR11XX_SYSTEM_IRQ_TIMEOUT) == LR11XX_SYSTEM_IRQ_TIMEOUT) {
		on_rx_timeout();
	}
}


