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

	  Herby& createStepper(int type, int pin1, int pin2);			// delete default Stepper and add CustomStepper
	  Herby& createDC(int pin1, int pin2, int speedPin);			// delete default DC-Motor and add CustomDC
	  Herby& initI2C(byte masterAddress, byte ownAddress);			// overwrite default I2C Addresses with CustomAddresses
	  Herby& addHead(HerbyHead* head);								// add HerbyHead instance
	  Herby& addParser(HerbyProtocolParser* parser);				// add HerbyProtocolParser instance

	  void begin();				// put all left init code needed here and call as final method in the main setup
	  void update();			// ONLY CALL THIS IN MAIN LOOP, update Herby in every loop, controlling StateMachine.
	  void runStepper();		// Call this in ISR of Timer activated with activateTimer1()

	private:
	  typedef enum { IDLE, INTERRUPTED, AUTOMATIC, MOVE, WORK, COLLECTING_DATA, SENDING_DATA } states; 	//States of Main-Herby

	  void activateTimer1();				// set HW-Timer for AxelStepper

	  void resetStateData();				// set all stateData struct member to 0 or false
	  void collectData();					// get all Data from HearbyHead and write to stateData

	  void setGridTarget(int posStepper, int posDC);	// set new TargetPosition in Grid
	  void setGridTargetAuto();							// new TargetPostion set by an automatic routine
	  bool runAxisMotors();								// run AxisMotors while in MOVE state. Mainly needed for DC to move. Checks also if Stepper is finished

	  bool sendData();									// send current stateData to I2C-Master
	  static void receiveData(int howMany);				// called when I2C Interrupt occurs

	  byte _masterAddress;					// I2C Adress of Master
	  byte _ownAddress;						// Own I2C Adress

	  stateData _stateData;					// struct for Sensor Data Exchange

	  HerbyHead* _head;							// Pointer for HerbyHead instance
	  static HerbyProtocolParser* _parser;		// Pointer for HerbyProtocolParser instance
	  DCMotor* _axisDC;							// Pointer for Axis-DCMotor
	  AccelStepper* _axisStepper;				// Pointer for AxisAccelStepper

	  static volatile bool _interrupted;	// flag for signalizing incoming I2C message
	  bool _motorsSetUp;					// check if new positions for the motors are set up
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
