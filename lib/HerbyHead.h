/*
 * HerbyHead.h
 *
 *  Created on: Jan 17, 2015
 *      Author: johannes
 */

#ifndef HERBYHEAD_H_
#define HERBYHEAD_H_

#include <Arduino.h>
#include <AccelStepper.h>
#include "HerbyConfig.h"

class HerbyHead
{
	public:
		HerbyHead();

		HerbyHead& createStepper(int type, int pin1, int pin2);		// overwrite default pins with custom pins
		HerbyHead& createWaterPump(int pin);						// overwrite default pins with custom pins
		HerbyHead& createLights(int pin);							// overwrite default pins with custom pins
		HerbyHead& createTemperature(int pin);						// overwrite default pins with custom pins
		HerbyHead& createHumidity(int pin);							// overwrite default pins with custom pins
		HerbyHead& createBrightness(int pin);						// overwrite default pins with custom pins
		HerbyHead& setHumidityPeriod(unsigned long period);			// overwrite default time with custom time
		HerbyHead& setBrightnessPeriod(unsigned long period);		// overwrite default time with custom time
		HerbyHead& setTemperaturePeriod(unsigned long period);		// overwrite default time with custom time
		HerbyHead& setPourPeriod(unsigned long period);				// overwrite default time with custom time

		// get methods, called by collectData of MainHerby
		int getHumidity() const;
		int getBrightness() const;
		int getTemperature() const;

		void begin();				// put all left init code needed here and call as final method in the main setup
		bool doWork();				// like update() of MainHerby. Called in loop while MainHerby is in WORK state.
		void runStepper();			// Call this in ISR of Timer.

	private:
		typedef enum {	IDLE,
						DRILL_IN,
						DRILL_OUT,
						HUMIDITY_CHECK,
						BRIGHTNESS_CHECK,
						POUR } states;		// sates of HearbyHead

		states _state;				// current state of HerbyHead
		bool _finished;				// flag for signaling that HerbyHead is finished. Return Value of doWork() -> Used in StateMachine of MainHerby.
		bool _lightOn;				// Light switch
		AccelStepper* _stepper;		// Z-Axis Stepper

		// Timer variables. Used for non blocking Sensor Measures etc.
		bool _timerStarted;
		unsigned long _now;
		unsigned long _timer;

		// time periods of sensor measures
		unsigned long _humidityPeriod;
		unsigned long _brightnessPeriod;
		unsigned long _temperaturePeriod;
		unsigned long _pourPeriod;

		// sensor and actor pins
		int _pinWaterPump;
		int _pinLights;
		int _pinBrightness;
		int _pinHumidity;

		// last data read from sensors
		uint16_t _humidityValue;
		uint16_t _brightnessValue;
		uint16_t _temperatureValue;
		uint16_t _probeCounter;			// used to do incremental mean calculation

		// sensor methods
		bool checkHumidity();
		bool checkBrightness();
		bool checkTemperature();

		// actor methods
		bool pour();
		void setDrillTarget(int drillPos);
		bool drillToTarget();
		void toggleLight();
		void resetValues();
};


#endif /* HERBYHEAD_H_ */
