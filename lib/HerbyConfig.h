/*
 * HerbyConfig.h
 *
 *  Created on: Jan 17, 2015
 *      Author: johannes
 */

#ifndef HERBYCONFIG_H_
#define HERBYCONFIG_H_

// All default pins are set here. You can change them here or use the "createXYZ" methods to set custom pins.
enum pins {
	AXIS_STEPPER_PIN1 = 5,
	AXIS_STEPPER_PIN2 = 4,
	AXIS_DC_PIN1 = 10,
	AXIS_DC_PIN2 = 11,
	AXIS_DC_SPEED_PIN = 9,
	HEAD_STEPPER_PIN1 = 7,
	HEAD_STEPPER_PIN2 = 6,
	HEAD_WATER_PUMP_PIN = 1,
	HEAD_LIGHT_PIN = 8,
	HEAD_HUMIDITY_PIN = A0,
	HEAD_BRIGHTNESS_PIN = A1,
	HEAD_TEMPERATURE_PIN = A2
};

// All default configuration is done here.
enum config_values {
	I2C_OWN_ADDRESS = 8,
	I2C_MASTER_ADDRESS = 9,
	AXIS_DC_STEP_PERIOD = 200,
	AXIS_STEPPER_TYPE = 1,
	AXIS_STEPPER_SPEED = 500,
	AXIS_STEPPER_MAX_SPEED = 2000,
	AXIS_STEPPER_MAX_STEPS = 14500,
	HEAD_STEPPER_TYPE = 1,
	HEAD_STEPPER_SPEED = 2000,
	HEAD_STEPPER_MAX_SPEED = 2000,
	HEAD_STEPPER_MAX_STEPS = 5000,
	HEAD_STEPPER_GRID_STEPS = 3,
	AXIS_STEPPER_GRID_STEPS = 4,
	AXIS_DC_GRID_STEPS = 3,
	DEFAULT_HUMIDITY_PERIOD = 1000,
	DEFAULT_POUR_PERIOD = 1000,
	DEFAULT_LIGHT_PERIOD = 1000,
	DEFAULT_TEMPERATURE_PERIOD = 1000
};

#endif /* HERBYCONFIG_H_ */
