#include "stormwater_pump.h"

void stormwater_pump_init(void) {
	gpio_reset_pin(ESP_PUMP);
	gpio_set_direction(ESP_PUMP, GPIO_MODE_OUTPUT);
}

void stormwater_pump_set(uint8_t opt) {
	if(opt == 1) {
		gpio_set_level(ESP_PUMP, 1);
	}
	else {
		gpio_set_level(ESP_PUMP, 0);
	}

}


