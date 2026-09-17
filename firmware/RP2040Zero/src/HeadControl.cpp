/*
 * HeadControl.cpp
 *
 *  Created on: 14 Sept 2026
 *      Author: jondurrant
 */

#include "HeadControl.h"
#include <stdio.h>

HeadControl * HeadControl::pSelf = NULL;

HeadControl::HeadControl() {
	// TODO Auto-generated constructor stub

}

HeadControl::~HeadControl() {
	// TODO Auto-generated destructor stub
}


HeadControl * HeadControl::singleton(){
	if (pSelf == NULL){
		pSelf = new HeadControl;
	}
	return pSelf;
}

void HeadControl::init(){
	// Assign fields individually: Servo has volatile members, so whole-struct
	// assignment would use the (deleted) implicit move/copy assignment operator.
	xServoPan.gpio          = PWM3B;
	xServoPan.period_usec   = 20000u;
	xServoPan.duty_min_usec = 700;
	xServoPan.duty_max_usec = 2300u;
	xServoPan.callback       = callback;

	xServoTilt.gpio          = PWM5B;
	xServoTilt.period_usec   = 20000u;
	xServoTilt.duty_min_usec = 700;
	xServoTilt.duty_max_usec = 2300u;
	xServoTilt.callback       = callback;

	servo_init(&xServoPan);
	servo_init(&xServoTilt);
    servo_set_rad(&xServoPan, M_PI/2.0);
    servo_set_rad(&xServoTilt, M_PI/2.0 + 0.4);
	sleep_ms(250);
	servo_set_rad(&xServoTilt, M_PI/2.0 + 0.3);
}

void HeadControl::rotate(float rad){
	float r = rad + (M_PI / 2.0) - 0.1;
	//printf("Attempt rotate to %f radians\n", r);
	if ((r < 0.0) || (r > M_PI)){
		return;
	}
	//servo_time_to_rad(&xServoPan, r, 1000000, ease_sin, NULL);
	xCancel = false;
	xCurrentRotation = rad;
	xMovingPan = true;
	servo_time_to_rad(&xServoPan, r, 100000, ease_in_expo, &xCancel);
}	


void HeadControl::rotateSlow(float rad){
	if (xMovingPan){
		return;
	}
	float r = rad + (M_PI / 2.0) - 0.1;
	//printf("Attempt slow rotate to %f radians\n", r);
	if ((r < 0.0) || (r > M_PI)){
		return;
	}
	xCancel = true;
	xCurrentRotation = rad;
	xMovingPan = true;
	servo_time_to_rad(&xServoPan, r, 2000000, ease_sin, &xCancel);
}

float HeadControl::getCurrentRotation(){
	return xCurrentRotation;
}

void HeadControl::callback(Servo* s){
	if (pSelf != NULL){
		pSelf->callbackHandler(s);
	}
}

void HeadControl::callbackHandler(Servo* s){
	if (s == &xServoPan){
		xMovingPan = false;
	} else if (s == &xServoTilt){
		xMovingTilt = false;
	}
}

void HeadControl::wakeAnim(){
	xMovingTilt = true;
	xCancelTilt = false;
	servo_time_to_rad(&xServoTilt, M_PI/2.0 + 0.4, 500000, ease_in_expo, &xCancelTilt);
	servo_time_to_rad(&xServoTilt, M_PI/2.0 + 0.3, 1000000, ease_in_expo, &xCancelTilt);
}

void HeadControl::sleepAnim(){
	xCancel = false;
	xCurrentRotation = 0.0;
	xMovingPan = true;
	servo_time_to_rad(&xServoPan, (M_PI / 2.0) - 0.1, 2000000, ease_sin, &xCancel);

	xCancelTilt = false;
	xMovingTilt = true;
	servo_time_to_rad(&xServoTilt, M_PI/2.0 -0.3, 2000000, ease_sin, &xCancelTilt);

	printf("Sleep\n");
}