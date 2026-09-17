/*
 * StateEngine.cpp
 *
 *  Created on: 14 Sept 2026
 *      Author: jondurrant
 */

#include "StateEngine.h"
#include "HeadControl.h"


StateEngine *StateEngine::pSelf = NULL;

StateEngine *StateEngine::singleton(){
	if (pSelf == NULL){
		pSelf = new StateEngine;
	}
	return pSelf;
}

StateEngine::StateEngine() {
	// TODO Auto-generated constructor stub

}

StateEngine::~StateEngine() {
	// TODO Auto-generated destructor stub
}

void StateEngine::setEyes(LedEye *l, LedEye *r){
	pLeft = l;
	pRight = r;
}

void StateEngine::setNoStaticTarget(){
	xStaticTS = 0;
}

void StateEngine::setStaticTarget(float rad, float mtrs){
	xStaticRad = rad;
	xStaticMtrs = mtrs;
	xStaticTS = to_ms_since_boot (get_absolute_time());
}

void StateEngine::setNumTargets(uint t){
	xTargets = t;
}

void StateEngine::setNoMovingTarget(){
	xMovingTS = 0;
	xMovingRad = 0.0;
	xMovingMtrs = 0.0;
}

void StateEngine::setMovingTarget(float rad, float mtrs){
	// Both xMovingRad and rad lie within [-M_PI/2, M_PI/2], so a plain
	// subtraction gives the difference without needing to wrap.
	xMovingRadDiff = fabsf(xMovingRad - rad);
	//printf("Setting moving target: rad=%f, prev=%f, radDiff=%f\n", rad, xMovingRad, xMovingRadDiff);
	xMovingRad = rad;
	xMovingMtrs = mtrs;
	xMovingTS = to_ms_since_boot (get_absolute_time());
}


bool StateEngine::isSleep(){
	uint32_t now = to_ms_since_boot (get_absolute_time());
	if ( now > ( xStaticTS + HEAD_SLEEP_TIME) && (now > (xMovingTS + HEAD_SLEEP_TIME))){
	//if (now > (xMovingTS + HEAD_SLEEP_TIME)){
		return true;
	}
	//printf("Awake %lu < %lu or %lu < %lu\n", now, xStaticTS + HEAD_SLEEP_TIME, now, xMovingTS + HEAD_SLEEP_TIME);
	return false;
}

PicoLed::Color StateEngine::getPupil(){
	switch(xState){
		case SkullAwake:
			return PicoLed::RGB(0, 0 , 0xFF);
		case SkullAsleep:
			return PicoLed::RGB(0x90, 0 , 0);
		case SkullWatch:
			return PicoLed::RGB(0, 0xFF , 0);
		case SkullHunt:
			return PicoLed::RGB(0xFF, 0 , 0);
	}
	return PicoLed::RGB(0xFF, 0xFF , 0xFF);
}

PicoLed::Color StateEngine::getIris(){
	if (isSleep()){
		return PicoLed::RGB(0, 0 , 0);
	}
	float m = 5.0;
	if (hasStatic()) {
		m = fmin(m, xStaticMtrs);
	}
	if (hasMoving()) {
		m = fmin(m, xMovingMtrs);
	}
	if (m < 0.75){
		return PicoLed::RGB(0xFF, 0 , 0);
	}
	if (m < 1.0){
		return PicoLed::RGB(191, 63 , 0);
	}
	if (m < 1.5){
		return PicoLed::RGB(127, 127 , 0);
	}
	if (m < 2.0){
		return PicoLed::RGB(63, 191 , 0);
	}

	return PicoLed::RGB(0, 0xFF , 0);
}

bool StateEngine::hasMoving(){
	return xMovingTS != 0;
}

bool StateEngine::hasStatic(){
	return xStaticTS != 0;
}


void StateEngine::update(){
	if ((pLeft != NULL) && (pRight != NULL)){
		pLeft->setPupil(getPupil(), isSleep());
		pRight->setPupil(getPupil(), isSleep());

		pLeft->setIris(getIris(), EyeClockwise, xTargets);
		pRight->setIris(getIris(), EyeWithershins, xTargets);
	}
	
	if (!isSleep()){
		if (xState == SkullAsleep){
			uint32_t sinceMove = to_ms_since_boot (get_absolute_time()) - xMovingTS;
			if (sinceMove < 20 ){
				HeadControl::singleton()->wakeAnim();
				xState = SkullAwake;
				xWakeCount = 0;
			}	
		} else {

			if (hasMoving()) {
				if (xMovingRadDiff > 0.01) {
					//printf(">0.1 move to Rotating head to %f radians\n", xMovingRad);
					HeadControl::singleton()->rotate(xMovingRad);
				} else {
					uint32_t tsd = to_ms_since_boot (get_absolute_time()) - xMovingTS;
					if ((tsd > 300) && (xMovingRadDiff > 0.001)){
						//printf("Time since last move > 300ms, rotating head to %f radians\n", xMovingRad);
						HeadControl::singleton()->rotate(xMovingRad);
					}
				}
			} else if (hasStatic()) {
			HeadControl::singleton()->rotateSlow(xStaticRad);
			}
		}
	} else {
		if (xState != SkullAsleep){
			HeadControl::singleton()->sleepAnim();
			xState = SkullAsleep;
		}
	}
	
}

