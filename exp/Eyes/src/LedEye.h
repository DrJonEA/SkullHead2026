/*
 * LedEye.h
 *
 *  Created on: 7 Sept 2026
 *      Author: jondurrant
 */

#ifndef EXP_EYES_SRC_LEDEYE_H_
#define EXP_EYES_SRC_LEDEYE_H_

#include "PicoLed.hpp"

enum EyeAnimation {
	EyeStatic,
	EyeClockwise,
	EyeWithershins,
	EyeOff
};

class LedEye {
public:
	LedEye();
	virtual ~LedEye();

	void init(PicoLed::PicoLedController *p);

	void setPupil(PicoLed::Color c, bool fade = false);
	void setIris(PicoLed::Color c, EyeAnimation a = EyeStatic);

	void tick();
	void setStep(int i=0);

private:
	PicoLed::PicoLedController *pCtr = NULL;
	PicoLed::Color xIris, xPupil;
	EyeAnimation xAnimate;

	int xStep =0;
	bool xFade = false;
	int xFadeStep = 0;


	PicoLed::Color xOff = PicoLed::RGB(0,0,0);
};

#endif /* EXP_EYES_SRC_LEDEYE_H_ */
