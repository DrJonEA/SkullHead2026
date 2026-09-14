/*
 * main.cpp

 *
 *  Created on: 15 Jul 2026
 *      Author: jondurrant
 */


#include "pico/stdlib.h"
#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "PicoLed.hpp"
#include "AstroDroidConfig.h"
#include "LedEye.h"

#include "StateEngine.h"
#include "SensorControl.h"
#include "HeadControl.h"

#define LED_LEN 14

PicoLed::PicoLedController xNeopixels = PicoLed::addLeds<PicoLed::WS2812B>(
					pio1, 0,
					GP_WS2812B,
					LED_LEN,
					PicoLed::FORMAT_GRB);
PicoLed::PicoLedController xLeftLeds = xNeopixels.slice(0, 6);
PicoLed::PicoLedController xRightLeds = xNeopixels.slice(7, 13);

int main( void ) {

    stdio_init_all();
    sleep_ms(2000);
    printf("GO\n");

    xNeopixels.setBrightness(30);
    LedEye left;
    LedEye right;
    left.init(&xLeftLeds);
    right.init(&xRightLeds);
    left.setStep(2);


    HeadControl::singleton()->init();
    StateEngine::singleton()->setEyes(&left, &right);
    SensorControl::singleton()->init();

    for (;;){
    	StateEngine::singleton()->update();
		left.tick();
		right.tick();
		xNeopixels.show();
		sleep_ms(100);
    }

}
