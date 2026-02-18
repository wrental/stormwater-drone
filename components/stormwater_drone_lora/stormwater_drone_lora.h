#ifndef STORMWATER_DRONE_LORA_H
#define STORMWATER_DRONE_LORA_H

#include "lr1121_config.h"

// ESP GPIO PINS
#define ESP_CS			(GPIO_NUM_12)
#define ESP_CLK			(GPIO_NUM_13)
#define ESP_MOSI		(GPIO_NUM_14)
#define ESP_MISO		(GPIO_NUM_15)
#define ESP_RESET		(GPIO_NUM_16)
#define ESP_BUSY		(GPIO_NUM_17)
#define ESP_INT			(GPIO_NUM_18)

// ESP SPI CONFIG
#define ESP_SPI_HOST			(SPI2_HOST)
#define ESP_SPI_CLK_HZ		8 * 1000 * 1000 // 8MHz

// LR11XX IRQ
#define IRQ_MASK                                                                          \
    ( LR11XX_SYSTEM_IRQ_TX_DONE | LR11XX_SYSTEM_IRQ_RX_DONE | LR11XX_SYSTEM_IRQ_TIMEOUT | \
      LR11XX_SYSTEM_IRQ_HEADER_ERROR | LR11XX_SYSTEM_IRQ_CRC_ERROR | LR11XX_SYSTEM_IRQ_FSK_LEN_ERROR )

// LR11XX APP SETTINGS
#define IS_HOST			false
#define RX_TIMEOUT_VALUE	RX_CONTINUOUS
#define TX_TIMEOUT_VALUE	
#define PACKET_PREFIX_SIZE	0
#define SYNC_PACKET_THRESHOLD	64
#define TX_RX_TRANSITION_DELAY	10  // ms
#define ITERATION_DELAY		10  // ms

/*!
 * @brief initialize lora module and interrupt service routine
 */
void stormwater_drone_lora_init(void);

/*!
 * @brief when flag boolean set, start interrupt process; reads/writes packets
 */
void stormwater_drone_lora_irq_process(void);

/*!
 * @brief packet to be sent of length PAYLOAD_LENGTH (in bytes)
 */
uint8_t stormwater_drone_lora_send_packet[PAYLOAD_LENGTH];

/*!
 * @brief packet to be sent of length PAYLOAD_LENGTH (in bytes)
 */
uint8_t stormwater_drone_lora_receive_packet[PAYLOAD_LENGTH];

/*!
 * @brief interrupt detect flag
 */
bool stormwater_drone_lora_irq_flag;


#endif
