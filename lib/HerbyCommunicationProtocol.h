/*
 * HerbyCommunicationProtocol.h
 *
 *  Created on: Jan 17, 2015
 *      Author: johannes
 */

#ifndef HERBYCOMMUNICATIONPROTOCOL_H_
#define HERBYCOMMUNICATIONPROTOCOL_H_

#include <Arduino.h>

// START AND END PATTERNS

#define START_PATTERN			0xc5
#define END_PATTERN				0x5c

//// COMMANDS
// Movement from 0x10 to 0x1F;
#define MOVE_TO_POINT			0x10

// Head commands from 0x22
#define HUMIDITY				0x20
#define WATERPUMP_ON			0x27

#define BRIGHTNESS				0x30
#define LIGHTS_ON				0x37

#define TEMPERATURE				0x40


struct stateData {
	uint8_t gridPosStepper;
	uint8_t gridPosDC;
	bool humidityCheck;
	uint16_t humidityValue;
	bool pour;
	bool brightnessCheck;
	uint16_t brightnessValue;
	bool lightsOn;
	bool temperatureCheck;
	uint16_t temperatureValue;
};

#endif /* HERBYCOMMUNICATIONPROTOCOL_H_ */
