/*
 * LedEye.cpp
 *
 *  Created on: 7 Sept 2026
 *      Author: jondurrant
 */

#include "LedEye.h"

#define EYE_PUPIL 0
#define EYE_LEDS 7
#define EYE_IRIS 1

LedEye::LedEye() {
	// TODO Auto-generated constructor stub

}

LedEye::~LedEye() {
	// TODO Auto-generated destructor stub
}

void LedEye::init(PicoLed::PicoLedController *p){
	pCtr = p;
}

void LedEye::setPupil(PicoLed::Color c){
	if (pCtr != NULL){
		pCtr->setPixelColor(EYE_PUPIL,  c);
	}
}
void LedEye::setIris(PicoLed::Color c, EyeAnimation a){
	xAnimate = a;
	xIris = c;
	if (xAnimate == EyeStatic){
		tick();
	}
	if (a == EyeOff){
		xAnimate = EyeStatic;
		xIris = xOff;
		tick();
	}

}

void LedEye::tick(){
	if (xAnimate == EyeStatic){
		for (int i=EYE_IRIS; i < EYE_LEDS; i++){
			pCtr->setPixelColor(i,  xIris);
		}
	}

	if ((xAnimate == EyeClockwise) || (xAnimate == EyeWithershins)){
		if (xStep >= EYE_LEDS){
			xStep = EYE_IRIS;
		}
		for (int i=EYE_IRIS; i < EYE_LEDS; i++){
			int l = i;
			if (xAnimate == EyeWithershins){
				l = EYE_LEDS - i;
			}
			if (i == xStep){
				pCtr->setPixelColor(l,  xIris);
			} else {
				pCtr->setPixelColor(l,  xOff);
			}
		}
		xStep++;
	}

}

void LedEye::setStep(int i){
	xStep = i;
}
