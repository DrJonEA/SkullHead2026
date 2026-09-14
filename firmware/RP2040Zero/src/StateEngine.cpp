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


bool StateEngine::isSleep(){
	uint32_t now = to_ms_since_boot (get_absolute_time());
	return( now > ( xStaticTS + HEAD_SLEEP_TIME));
}

PicoLed::Color StateEngine::getPupil(){
	if (isSleep()){
		return PicoLed::RGB(0, 0 , 0);
	}
	return PicoLed::RGB(0, 0 , 0xFF);
}

PicoLed::Color StateEngine::getIris(){
	if (isSleep()){
		return PicoLed::RGB(0, 0 , 0);
	}
	if (xStaticMtrs < 0.5){
		return PicoLed::RGB(0xFF, 0 , 0);
	}
	if (xStaticMtrs < 0.75){
		return PicoLed::RGB(191, 63 , 0);
	}
	if (xStaticMtrs < 1.0){
		return PicoLed::RGB(127, 127 , 0);
	}
	if (xStaticMtrs < 1,5){
		return PicoLed::RGB(63, 191 , 0);
	}

	return PicoLed::RGB(0, 0xFF , 0);
}


void StateEngine::update(){
	if ((pLeft != NULL) && (pRight != NULL)){
		pLeft->setPupil(getPupil());
		pRight->setPupil(getPupil());

		pLeft->setIris(getIris(), EyeClockwise, xTargets);
		pRight->setIris(getIris(), EyeWithershins, xTargets);
	}
	if (!isSleep()){
		HeadControl::singleton()->rotate(xStaticRad);
	}
}

