/*
 * StateEngine.h
 *
 *  Created on: 14 Sept 2026
 *      Author: jondurrant
 */

#ifndef FIRMWARE_RP2040ZERO_SRC_STATEENGINE_H_
#define FIRMWARE_RP2040ZERO_SRC_STATEENGINE_H_

#include "pico/stdlib.h"
#include "PicoLed.hpp"
#include "LedEye.h"

#define HEAD_SLEEP_TIME (10 * 1000)

class StateEngine {
public:
	static StateEngine *singleton();
	virtual ~StateEngine();

	void setEyes(LedEye *l, LedEye *r);

	void setNoStaticTarget();
	void setStaticTarget(float rad, float mtrs);
	void setNumTargets(uint t);

	void update();

private:
	static StateEngine *pSelf;
	StateEngine();

	bool isSleep();
	PicoLed::Color getPupil();
	PicoLed::Color getIris();

	LedEye *pLeft;
	LedEye *pRight;

	uint32_t xStaticTS = 0;
	float xStaticRad = 0.0;
	float xStaticMtrs = 2.0;
	uint8_t xTargets = 0;

};

#endif /* FIRMWARE_RP2040ZERO_SRC_STATEENGINE_H_ */
