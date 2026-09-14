/*
 * HeadControl.h
 *
 *  Created on: 14 Sept 2026
 *      Author: jondurrant
 */

#ifndef FIRMWARE_RP2040ZERO_SRC_HEADCONTROL_H_
#define FIRMWARE_RP2040ZERO_SRC_HEADCONTROL_H_

#include "AstroDroidConfig.h"

extern "C" {
#include "servo.h"
}


class HeadControl {
public:
	static HeadControl * singleton();
	virtual ~HeadControl();

	void init();

	void rotate(float rad);

private:
	static HeadControl * pSelf;
	HeadControl();

	// Zero-initialised: servo_init() relies on unset fields (e.g. sec_per_60,
	// max_degrees, start_deg) being 0 to apply its own defaults.
	Servo xServoPan{};
	Servo xServoTilt{};
};

#endif /* FIRMWARE_RP2040ZERO_SRC_HEADCONTROL_H_ */
