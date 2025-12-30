/* name: stormwater-drone.cpp
 * auth: wrental
 * date: 2025-12-30
 * desc: Elon Univ. SrDsn - Stormwater Drone
 * 				drone-side ESP32 app for data TX, controls RX
 * 				built for ESP32-S3 with Waveshare Core 1121 HF
 */

#include <iostream>
#include "Arduino.h"

extern "C" void app_main(void) {
	initArduino();

	// Arduino setup:
	Serial.begin(115200);
	while(!Serial) {
		;
	}

	// Program setup:
	int x {0};
	
	// Arduino loop:
	while(1) {
		vTaskDelay(250 * portTICK_PERIOD_MS);
		x = x + 1;
		Serial.print("loop: ");
		Serial.println(x);
	}
}
