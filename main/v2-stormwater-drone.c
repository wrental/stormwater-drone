// name: v2-stormwater-drone.c
// date: 02-10-2026
// auth: wrental
// desc: version way more than 2 ngl

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_lora_1121.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"


// needed functions:
// - init()
// - getTemp()
// - getDO2()
// - getPH()

// LoRa setup:

void app_main(void)
{
	// program setup
	int x = 0;

	// program loop
	while(1) {
		x = x + 1;
		printf("loop: %d\n", x);
	}
}


