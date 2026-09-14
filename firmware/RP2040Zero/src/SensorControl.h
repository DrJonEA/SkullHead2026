/*
 * SensorControl.h
 *
 *  Created on: 14 Sept 2026
 *      Author: jondurrant
 */

#ifndef FIRMWARE_RP2040ZERO_SRC_SENSORCONTROL_H_
#define FIRMWARE_RP2040ZERO_SRC_SENSORCONTROL_H_

#include "pico/stdlib.h"
#include "KW307_api.h"

class SensorControl {
public:
	static SensorControl * singleton();
	virtual ~SensorControl();

	void init();

private:
	static SensorControl * pSelf;
	SensorControl();

	static void onRadar(const KW307Frame_t* f);
	void handleRadar(const KW307Frame_t* f);

};

#endif /* FIRMWARE_RP2040ZERO_SRC_SENSORCONTROL_H_ */
