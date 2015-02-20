/*
 * HerbyProtocolParser.cpp
 *
 *  Created on: Jan 20, 2015
 *      Author: johannes
 */

#include "HerbyProtocolParser.h"

HerbyProtocolParser::HerbyProtocolParser()
{}

bool HerbyProtocolParser::fillReceiveBuffer(int howMany) {
	for (int i = 0; i < howMany; i++) {
		_receiveBuffer[i] = Wire.read();
		Serial.println(_receiveBuffer[i], HEX);
	}
	return validate();
}

bool HerbyProtocolParser::validate() {
	uint8_t startByte = _receiveBuffer[0];

	if  (startByte != START_PATTERN) return false;

	uint8_t length = _receiveBuffer[1];
	uint8_t parityCheck = _receiveBuffer[0];

	for (int i = 1; i < length-1; i++) {
		parityCheck ^= _receiveBuffer[i];
	}

	if (parityCheck != _receiveBuffer[length - 1]) return false;

	return true;
}

void HerbyProtocolParser::extractBuffer(stateData& stateData) {
	uint8_t dataLength = _receiveBuffer[1] - 5;		// substract the static bits of the protocol

	for(int i = 2; i < dataLength; i += 3 ) {
		uint8_t command = _receiveBuffer[i];

		switch(command) {
			case MOVE_TO_POINT:
				stateData.gridPosStepper = _receiveBuffer[i + 1];
				stateData.gridPosDC = _receiveBuffer[i + 2];
				break;
			case HUMIDITY:
				stateData.humidityCheck = true;
				break;
			case WATERPUMP_ON:
				stateData.pour = true;
				break;
			case BRIGHTNESS:
				stateData.brightnessCheck = true;
				break;
			case LIGHTS_ON:
				stateData.lightsOn = true;
				break;
			case TEMPERATURE:
				stateData.temperatureCheck = true;
				break;
			default:
				break;
		}
	}
}

void HerbyProtocolParser::insert(stateData& stateData) {
	int cmdCounter = 2;	// always insert grid position

	_sendBuffer[0] = START_PATTERN;
	addCommandToSendBuffer(cmdCounter, MOVE_TO_POINT, stateData.gridPosStepper, stateData.gridPosDC);
	cmdCounter += 3;

	uint8_t lowByte;
	uint8_t highByte;

	if (stateData.brightnessCheck) {
		lowByte = stateData.brightnessValue & 0xFF;
		highByte = (stateData.brightnessValue >> 8) & 0xFF;
		addCommandToSendBuffer(cmdCounter, BRIGHTNESS, lowByte, highByte);
		cmdCounter += 3;
	}
	if (stateData.humidityCheck) {
		lowByte = stateData.brightnessValue & 0xFF;
		highByte = (stateData.brightnessValue >> 8) & 0xFF;
		addCommandToSendBuffer(cmdCounter, HUMIDITY, lowByte, highByte);
		cmdCounter += 3;
	}
	if (stateData.temperatureCheck) {
		lowByte = stateData.brightnessValue & 0xFF;
		highByte = (stateData.brightnessValue >> 8) & 0xFF;
		addCommandToSendBuffer(cmdCounter, TEMPERATURE, lowByte, highByte);
		cmdCounter += 3;
	}

	int length = cmdCounter + 3;			// last three static bytes in protocol

	_sendBuffer[1] = length;
	_sendBuffer[length - 2] = 0x00;			// future use
	_sendBuffer[length - 3] = 0x00;			// future use

	uint8_t parity = _sendBuffer[0];
	for (int i = 1; i<length-1; i++){
		parity ^= _sendBuffer[i];
	}
	_sendBuffer[length-1] = parity;

}

void HerbyProtocolParser::addCommandToSendBuffer(int pos, uint8_t cmd, uint8_t dataLowByte, uint8_t dataHighByte) {
	_sendBuffer[pos] = cmd;
	_sendBuffer[pos + 1] = dataLowByte;
	_sendBuffer[pos + 2] = dataHighByte;
}

bool HerbyProtocolParser::send(byte adress){

		Wire.beginTransmission(adress);
		Wire.write(_sendBuffer, sizeof _sendBuffer);
		if (Wire.endTransmission() == 0) return true;

		return false;

}


