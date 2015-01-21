/*
 * HerbyHead.cpp
 *
 *  Created on: Jan 20, 2015
 *      Author: johannes
 */

#include "HerbyHead.h"

HerbyHead::HerbyHead() :
	_state(IDLE), _finished(false), _timerStarted(false), _stepper(0), _now(0), _timer(0),
	_humidityPeriod(DEFAULT_HUMIDITY_PERIOD), _brightnessPeriod(DEFAULT_LIGHT_PERIOD),
	_temperaturePeriod(DEFAULT_TEMPERATURE_PERIOD), _pourPeriod(DEFAULT_POUR_PERIOD),
	_pinWaterPump(HEAD_WATER_PUMP_PIN), _pinLights(HEAD_LIGHT_PIN),
	_pinBrightness(HEAD_BRIGHTNESS_PIN), _pinHumidity(HEAD_HUMIDITY_PIN),
	_humidityValue(0), _brightnessValue(0), _temperatureValue(0)
{
	_stepper = new AccelStepper(HEAD_STEPPER_TYPE, HEAD_STEPPER_PIN1, HEAD_STEPPER_PIN2);
}

HerbyHead& HerbyHead::createStepper(int type, int pin1, int pin2) {
	delete _stepper;
	_stepper = new AccelStepper(type, pin1, pin2);
	_stepper->setMaxSpeed(HEAD_STEPPER_MAX_SPEED);

	return *this;
}

void HerbyHead::runStepper() {
	_stepper->runSpeedToPosition();
}



HerbyHead& HerbyHead::createWaterPump(int pin) {
	_pinWaterPump = pin;

	return *this;
}

HerbyHead& HerbyHead::createLights(int pin) {
	_pinLights = pin;

	return *this;
}

HerbyHead& HerbyHead::createHumidity(int pin) {
	_pinHumidity = pin;

	return *this;
}

HerbyHead& HerbyHead::createBrightness(int pin) {
	_pinBrightness = pin;

	return *this;
}

HerbyHead& HerbyHead::setHumidityPeriod(unsigned long period) {
	_humidityPeriod = period;

	return *this;
}

HerbyHead& HerbyHead::setBrightnessPeriod(unsigned long period) {
	_brightnessPeriod = period;

	return *this;
}

HerbyHead& HerbyHead::setPourPeriod(unsigned long period) {
	_pourPeriod = period;

	return *this;
}

void HerbyHead::begin() {
	pinMode(_pinWaterPump, OUTPUT);
	pinMode(_pinLights, OUTPUT);
	pinMode(_pinHumidity, INPUT);
	pinMode(_pinBrightness, INPUT);
	_timer = millis();
}

void HerbyHead::resetValues() {
	_humidityValue = 0;
	_brightnessValue = 0;
}

bool HerbyHead::checkHumidity() {
	if(!_timerStarted) {
		_timer = millis();
		_timerStarted = true;
	}
	else {
		if ( (_now - _timer) <= _humidityPeriod )  {
//			TODO: real pouring
			return true;
		}
		else {
			_timerStarted = false;
			return false;
		}
	}

	return true;
}

bool HerbyHead::checkBrightness() {
	return false;
}

bool HerbyHead::pour() {
	if(!_timerStarted) {
		_timer = millis();
		_timerStarted = true;
	}
	else {
		if ( (_now - _timer) <= _pourPeriod )  {
//			TODO: real pouring
			return true;
		}
		else {
			_timerStarted = false;
			return false;
		}
	}

	return true;
}

void HerbyHead::setDrillTarget(int drillPos){
	// Set up HeadStepper
	unsigned long newDrillPos = map(drillPos, 0, HEAD_STEPPER_GRID_STEPS, 0, HEAD_STEPPER_MAX_STEPS);
	_stepper->moveTo(newDrillPos);
	_stepper->setSpeed(HEAD_STEPPER_SPEED);
}

bool HerbyHead::drillToTarget() {
	if( _stepper->distanceToGo() == 0) return false;
	return true;
}

void HerbyHead::toggleLight() {

}

int HerbyHead::getHumidity() const {
	return _humidityValue;
}

int HerbyHead::getBrightness() const {
	return _brightnessValue;
}

int HerbyHead::getTemperature() const {
	return _temperatureValue;
}

bool HerbyHead::doWork() {

	_now = millis();

	switch(_state) {
		case IDLE:
			_finished = false;
			_state = HUMIDITY_CHECK;
			break;
		case HUMIDITY_CHECK:
			if (!checkHumidity()) _state = DRILL_IN;
			setDrillTarget(3);
			break;
		case DRILL_IN:
			if(!drillToTarget()) _state = POUR;
			break;
		case POUR:
			if (!pour()) _state = DRILL_OUT;
			setDrillTarget(0);
			break;
		case DRILL_OUT:
			if(!drillToTarget()){
				_finished = true;
				_state = IDLE;
			}
			break;
		default:
			break;
	}

	return _finished;
}
