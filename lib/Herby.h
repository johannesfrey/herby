/*
 * Herby.h
 *
 *  Created on: Jan 17, 2015
 *      Author: johannes
 */

#ifndef HERBY_H_
#define HERBY_H_

#include <Arduino.h>
#include <Wire.h>
#include <DCMotor.h>
#include <AccelStepper.h>
#include "HerbyConfig.h"
#include "HerbyHead.h"
#include "HerbyProtocolParser.h"
#include "HerbyCommunicationProtocol.h"

class Herby
  {
	public:
	  Herby();

	  Herby& createStepper(int type, int pin1, int pin2);
	  Herby& createDC(int pin1, int pin2, int speedPin);
	  Herby& initI2C(byte masterAddress, byte ownAddress);
	  Herby& addHead(HerbyHead* head);
	  Herby& addParser(HerbyProtocolParser* parser);

	  void begin();
	  void update();
	  void runStepper();

	private:
	  typedef enum { IDLE, INTERRUPTED, AUTOMATIC, MOVE, WORK, COLLECTING_DATA, SENDING_DATA } states;

	  void activateTimer1();

	  void resetStateData();
	  void collectData();

	  void setGridTarget(int posStepper, int posDC);
	  void setGridTargetAuto();
	  bool runAxisMotors();

	  bool sendData();
	  static void receiveData(int howMany);

	  byte _masterAddress;
	  byte _ownAddress;

	  stateData _stateData;

	  HerbyHead* _head;
	  static HerbyProtocolParser* _parser;
	  DCMotor* _axisDC;
	  AccelStepper* _axisStepper;

	  static volatile bool _interrupted;	// flag for signalizing incoming I2C message
	  bool _motorsSetUp;			// check if new positions for the motors are set up
	  bool _dcMoving;
	  bool _stepperMoving;

	  int _stepsStepper;			// in automatic mode, how many grid fields to move in one step
	  int _stepsDC;					// in automatic mode, how many grid fields to move in one step
	  int _posStepperAuto;			// current position in automatic mode
	  int _posDCAuto;				// current position in automatic mode
	  int _lastPosDC;				// save the latest grid position of dc motor to calculate steps to move

	  unsigned long _now;			// time for current loop cycle
	  unsigned long _timerDC;		// current time when motors are getting set up
	  unsigned long _periodDC;		// period the dc motor runs
	  states _state;				// current state of Herby
  };



#endif /* HERBY_H_ */
