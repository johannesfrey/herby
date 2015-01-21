/*
 * Herby.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: johannes
 */

#include "Herby.h"

HerbyProtocolParser* Herby::_parser = new HerbyProtocolParser();

volatile bool Herby::_interrupted = false;

void Herby::receiveData(int howMany) {
	if (_interrupted) return;

	if(_parser->fillReceiveBuffer(howMany)) {
		_interrupted = true;
	}
}

Herby::Herby() :
	_masterAddress(I2C_MASTER_ADDRESS), _ownAddress(I2C_OWN_ADDRESS),
	_head(0), _axisDC(0), _axisStepper(0), _motorsSetUp(false),
	_dcMoving(false), _stepperMoving(false), _stepsStepper(1),
	_stepsDC(1), _posStepperAuto(0), _posDCAuto(0), _lastPosDC(0),
	_now(0), _timerDC(0), _periodDC(0),  _state(IDLE)
{
		_axisDC = new DCMotor(AXIS_DC_PIN1, AXIS_DC_PIN2, AXIS_DC_SPEED_PIN);
		_axisStepper = new AccelStepper(AXIS_STEPPER_TYPE, AXIS_STEPPER_PIN1, AXIS_STEPPER_PIN2);
		// TODO default head & parser
}

void Herby::resetStateData() {
	_stateData.gridPosStepper = 0;
	_stateData.gridPosDC = 0;
	_stateData.humidityCheck = false;
	_stateData.humidityValue = 0;
	_stateData.pour = false;
	_stateData.brightnessCheck = false;
	_stateData.brightnessValue = 0;
	_stateData.lightsOn = false;
	_stateData.temperatureCheck = false;
	_stateData.temperatureValue = 0;
}

Herby& Herby::createStepper(int type, int pin1, int pin2) {
	delete _axisStepper;
	_axisStepper = new AccelStepper(type, pin1, pin2);
	_axisStepper->setMaxSpeed(AXIS_STEPPER_MAX_SPEED);

	return *this;
}

Herby& Herby::createDC(int pin1, int pin2, int speedPin) {
	delete _axisDC;
	_axisDC = new DCMotor(pin1, pin2, speedPin);

	return *this;
}

Herby& Herby::initI2C(byte masterAddress, byte ownAddress) {
	_masterAddress = masterAddress;
	_ownAddress = ownAddress;

	return *this;
}

Herby& Herby::addHead(HerbyHead* head) {
	_head = head;

	return *this;
}


Herby& Herby::addParser(HerbyProtocolParser* parser) {
	_parser = parser;

	return *this;
}


void Herby::begin() {
	resetStateData();
	Wire.begin(_ownAddress);
	Wire.onReceive(receiveData);
	_axisDC->begin();
	activateTimer1();
	_timerDC = millis();
}

void Herby::activateTimer1() {
	// Set up Timer1 for stepper
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;

	TCCR1B = bit(WGM12) | bit(CS12);
	OCR1A = 1;
	TIMSK1 = bit(OCIE1A);
}

void Herby::runStepper() {
	_axisStepper->runSpeedToPosition();
}

bool Herby::runAxisMotors() {
	_stepperMoving = _axisStepper->distanceToGo() == 0 ? false : true;
	_now = millis();

// Move DC over period
	if ( (_now - _timerDC) <= _periodDC) {
		// TODO: refactor DC to run()
		_axisDC->move();
	} else {
		_axisDC->stop();
		_dcMoving = false;
	}

	// Both motors arrived, reenable motorSetUp
	if (!_stepperMoving && !_dcMoving) {
		return true;
	}

	return false;
}

void Herby::setGridTarget(int posStepper, int posDC) {
	// Set up DC
	_stateData.gridPosStepper = posStepper;
	_stateData.gridPosDC = posDC;

	int stepsToMoveDCMotor = _lastPosDC - posDC;
	_periodDC = AXIS_DC_STEP_PERIOD * abs(stepsToMoveDCMotor);

	if (stepsToMoveDCMotor < 0) {
		// TODO: refactor dc motor
		_axisDC->setDirection(1);
	}
	else {
		_axisDC->setDirection(0);
	}

	_axisDC->start();
	_dcMoving = true;
	_lastPosDC = posDC;

	// Set up Stepper
	unsigned long newPosStepperMotor = map(posStepper, 0, AXIS_STEPPER_GRID_STEPS, 0, AXIS_STEPPER_MAX_STEPS);
	_axisStepper->moveTo(newPosStepperMotor);
	_axisStepper->setSpeed(AXIS_STEPPER_SPEED);

	_timerDC = millis();
}

void Herby::setGridTargetAuto() {

	setGridTarget(_posStepperAuto, _posDCAuto);
	_posDCAuto += _stepsDC;

	if (_posDCAuto > AXIS_DC_GRID_STEPS || _posDCAuto < 0) {
		_stepsDC *= -1;
		_posStepperAuto += _stepsStepper;

		if (_posDCAuto > AXIS_DC_GRID_STEPS)
			_posDCAuto = AXIS_DC_GRID_STEPS;
		else
			_posDCAuto = 0;
	}

	if (_posStepperAuto > AXIS_STEPPER_GRID_STEPS || _posStepperAuto < 0) {
		_stepsStepper *= -1;
//			posDCMotor += stepsDCMotor;

		if (_posStepperAuto > AXIS_STEPPER_GRID_STEPS)
			_posStepperAuto = 3;
		else
			_posStepperAuto = 1;
	}
}

void Herby::sendData() {
//	TODO
}

void Herby::collectData() {
	_stateData.brightnessCheck = true;
	_stateData.brightnessValue = _head->getBrightness();
	_stateData.humidityCheck = true;
	_stateData.humidityValue = _head->getHumidity();
	_stateData.temperatureCheck = true;
	_stateData.temperatureValue = _head->getTemperature();
}

void Herby::update() {
	switch(_state) {
		case IDLE:
			if (_interrupted) {
				_state = INTERRUPTED;
			}
			else _state = AUTOMATIC;
			break;
		case INTERRUPTED:
			resetStateData();
			_parser->extractBuffer(_stateData);
			setGridTarget(_stateData.gridPosStepper, _stateData.gridPosDC);
			_interrupted = false;
			_state = MOVE;
			break;
		case AUTOMATIC:
			resetStateData();
			setGridTargetAuto();
			_state = MOVE;
			break;
		case MOVE:
			if (runAxisMotors()) _state = WORK;
			break;
		case WORK:
			if (_head->doWork()) _state = COLLECTING_DATA;
			break;
		case COLLECTING_DATA:
			collectData();
			_state = SENDING_DATA;
			break;
		case SENDING_DATA:
			_state = IDLE;
			break;
	}
}
