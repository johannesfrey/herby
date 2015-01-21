/*
 * Main.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: johannes
 */

#include <Arduino.h>
#include <HerbyConfig.h>
#include <Herby.h>
#include <HerbyHead.h>

Herby herby;
HerbyHead herbyHead;

ISR(TIMER1_COMPA_vect)
{
	herby.runStepper();
	herbyHead.runStepper();
}

void setup() {
	herbyHead.createStepper(HEAD_STEPPER_TYPE, HEAD_STEPPER_PIN1, HEAD_STEPPER_PIN2)
			 .createWaterPump(HEAD_WATER_PUMP_PIN)
			 .createHumidity(HEAD_HUMIDITY_PIN)
			 .createLights(HEAD_LIGHT_PIN)
			 .createBrightness(HEAD_BRIGHTNESS_PIN)
			 .begin();

	herby.createStepper(AXIS_STEPPER_TYPE, AXIS_STEPPER_PIN1, AXIS_STEPPER_PIN2)
		 .createDC(AXIS_DC_PIN1, AXIS_DC_PIN2, AXIS_DC_SPEED_PIN)
		 .initI2C(I2C_MASTER_ADDRESS, I2C_OWN_ADDRESS)
		 .addHead(&herbyHead)
		 .begin();

}

void loop() {
	herby.update();
}
