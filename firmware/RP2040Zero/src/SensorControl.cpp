/*
 * SensorControl.cpp
 *
 *  Created on: 14 Sept 2026
 *      Author: jondurrant
 */

#include "SensorControl.h"
#include "StateEngine.h"
#include <cstdio>

SensorControl * SensorControl::pSelf = NULL;

SensorControl * SensorControl::singleton(){
	if (pSelf == NULL){
		pSelf = new SensorControl;
	}
	return pSelf;
}

SensorControl::SensorControl() {
}

void SensorControl::init(){
	if (kw307_init() != KW307_OK) {
			printf("KW307 Init Failed\n");
	}

	kw307_check_fw_version(300);
	kw307_set_output_default();
	kw307_set_gain               (5);              /* +12 dB */
	kw307_set_detect_range       (10, 1000);       /* 0.5~10 m */
	kw307_set_detect_fov         (120, 8);        /* 120 deg cone, shrink 8 deg/m */
	kw307_set_motion_sensitivity    (2, 0);
	kw307_set_stationary_sensitivity(1);
	kw307_set_moving_tracker_cnt(32, 8);    /* life 32f, show 8 */
	kw307_set_stationary_tracker_cnt(64, 8);    /* life 64f, show 8 */
	kw307_set_flag_hold_time (11);          /* presence flag hold 11 s */
	kw307_set_max_stationary_time(1, 20);          /* still cap on, 20 s */
	kw307_set_force_stationary   (true, 150, 40);  /* on, 150 cm, 40 deg */
	kw307_set_advanced_clutter_rejection(true,  300, 20);

	kw307_on_frame(SensorControl::onRadar);
}

SensorControl::~SensorControl() {
	// TODO Auto-generated destructor stub
}

void SensorControl::onRadar(const KW307Frame_t* f){
	singleton()->handleRadar(f);
}

void SensorControl::handleRadar(const KW307Frame_t* f){
	uint targets = 0;

	if (f->stationary[0].state == 1){
		float a = (float)f->stationary[0].angle_tenth/ 10.0;
		float rad = a * M_PI/180.0;
		float mtr = (float)f->stationary[0].distance_cm / 100.0;
		StateEngine::singleton()->setStaticTarget(rad, mtr);
		printf("Static %.2fRad, %.2fm\n", rad, mtr);
		targets++;
	} else {
		if (f->moving[0].state == 3){
			float a = (float)f->moving[0].angle_tenth/ 10.0;
			float rad = a * M_PI/180.0;
			float mtr = (float)f->moving[0].distance_cm / 100.0;
			StateEngine::singleton()->setStaticTarget(rad, mtr);
			printf("Moving %.2fRad, %.2fm\n", rad, mtr);
		} else {
			StateEngine::singleton()->setNoStaticTarget();
		}
	}

	for (int i=0; i < 2; i++){
	   if (f->moving[i].state == 3){
		   targets++;
	   }
	}

	printf("Handle Radar %u\n", targets);
	StateEngine::singleton()->setNumTargets(targets);
}
