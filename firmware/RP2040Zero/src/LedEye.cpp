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

void LedEye::setPupil(PicoLed::Color c, bool fade){
	xPupil = c;
	xFade = fade;
	if ((pCtr != NULL) && !fade){
		pCtr->setPixelColor(EYE_PUPIL,  c);
	}
}

void LedEye::setIris(PicoLed::Color c, EyeAnimation a, uint8_t param){
	xAnimate = a;
	xIris = c;
	xParam = param;
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
	if (!pCtr){
		return;
	}
	if (xAnimate == EyeStatic){
		for (int i=EYE_IRIS; i < EYE_LEDS; i++){
			pCtr->setPixelColor(i,  xIris);
		}
	}

	if ((xAnimate == EyeClockwise) || (xAnimate == EyeWithershins)){
		if (xStep >= EYE_LEDS){
			xStep = EYE_IRIS;
		}
		int numLights = 1;
		if (xParam > 1) {
			numLights = xParam;
		}
		for (int i=EYE_IRIS; i < EYE_LEDS; i++){
			pCtr->setPixelColor(i,  xOff);
		}
	
		for (int j = 0; j < numLights; j++){
			int l = (xStep + j) % (EYE_LEDS-1) + 1 ;
			if (xAnimate == EyeWithershins){
				l = EYE_LEDS - l;
			}
			for (int i=EYE_IRIS; i < EYE_LEDS; i++){
				if (l == i){
					pCtr->setPixelColor(l,  xIris);
				} 
			}
		}
		xStep++;
	}

	if (xFade){
		if (xFadeStep > 100){
			xFadeStep = 0;
		}
		//printf("Fade %d\n", xFadeStep);
		if (xFadeStep > 50){
			pCtr->fadePixel(EYE_PUPIL ,  xPupil,  0.1);
		} else {
			pCtr->fadePixel(EYE_PUPIL ,  xOff,  0.1);
		}

		xFadeStep++;
	}

}

void LedEye::setStep(int i){
	xStep = i;
}
