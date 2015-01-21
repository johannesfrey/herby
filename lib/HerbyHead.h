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

		HerbyHead& createStepper(int type, int pin1, int pin2);
		HerbyHead& createWaterPump(int pin);
		HerbyHead& createLights(int pin);
		HerbyHead& createTemperature(int pin);
		HerbyHead& createHumidity(int pin);
		HerbyHead& createBrightness(int pin);
		HerbyHead& setHumidityPeriod(unsigned long period);
		HerbyHead& setBrightnessPeriod(unsigned long period);
		HerbyHead& setTemperaturePeriod(unsigned long period);
		HerbyHead& setPourPeriod(unsigned long period);

		int getHumidity() const;
		int getBrightness() const;
		int getTemperature() const;

		void begin();
		bool doWork();
		void runStepper();

	private:
		typedef enum {	IDLE,
						DRILL_IN,
						DRILL_OUT,
						HUMIDITY_CHECK,
						BRIGHTNESS_CHECK,
						POUR } states;

		states _state;
		bool _finished;
		bool _timerStarted;
		AccelStepper* _stepper;

		unsigned long _now;
		unsigned long _timer;
		unsigned long _humidityPeriod;
		unsigned long _brightnessPeriod;
		unsigned long _temperaturePeriod;
		unsigned long _pourPeriod;

		int _pinWaterPump;
		int _pinLights;
		int _pinBrightness;
		int _pinHumidity;

		uint16_t _humidityValue;
		uint16_t _brightnessValue;
		uint16_t _temperatureValue;

		bool checkHumidity();
		bool checkBrightness();
		bool checkTemperature();

		bool pour();
		bool drillTo();
		void toggleLight();
		void resetValues();
};


#endif /* HERBYHEAD_H_ */
