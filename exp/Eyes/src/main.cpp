/*
 * main.cpp
 *
 *  Created on: 7 Sept 2026
 *      Author: jondurrant
 */

#include "pico/stdlib.h"
#include <stdio.h>

#include "PicoLed.hpp"

#include "AstroDroidConfig.h"
#include "LedEye.h"

#define LED_LEN 14

PicoLed::PicoLedController xNeopixels = PicoLed::addLeds<PicoLed::WS2812B>(
					pio1, 0,
					GP_WS2812B,
					LED_LEN,
					PicoLed::FORMAT_GRB);

#define COLOUR_COUNT 8
PicoLed::Color colours[COLOUR_COUNT] = {
		PicoLed::RGB(228,3 , 3),
		PicoLed::RGB(255, 140, 0),
		PicoLed::RGB(255, 237, 0),
		PicoLed::RGB(0, 128, 38),
		PicoLed::RGB(0, 76, 255),
		PicoLed::RGB(115, 41, 130),
		PicoLed::RGB(97, 57, 21),
		PicoLed::RGB(116, 215, 238)
};

int main( void ) {

    stdio_init_all();
    sleep_ms(2000);
    printf("GO\n");

    xNeopixels.setBrightness(30);
    PicoLed::PicoLedController xLeftLeds = xNeopixels.slice(0, 6);
    PicoLed::PicoLedController xRightLeds = xNeopixels.slice(7, 13);
    LedEye left;
    LedEye right;
    left.init(&xLeftLeds);
    right.init(&xRightLeds);
    left.setStep(2);


    for (;;){
    	for (int i=0; i < COLOUR_COUNT; i++){
    		//xNeopixels.fill( colours[i] );
    		left.setPupil(colours[i]);
    		right.setPupil(colours[i]);
    		left.setIris(colours[ (i+1) % COLOUR_COUNT], EyeWithershins);
    		right.setIris(colours[ (i+1) % COLOUR_COUNT], EyeClockwise);
    		for (int j=0; j < 20; j++) {
    			left.tick();
    			right.tick();
    			xNeopixels.show();
    			sleep_ms(500);
    		}

    	}
    }
}

