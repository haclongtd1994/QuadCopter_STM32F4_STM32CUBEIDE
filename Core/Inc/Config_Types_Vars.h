/*
 * Config_Types_Vars.h
 *
 *  Created on: Mar 14, 2020
 *      Author: PC
 */

#ifndef INC_CONFIG_TYPES_VARS_H_
#define INC_CONFIG_TYPES_VARS_H_

/* Include some library standard */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "main.h"

/* -------------------- CONFIGURATION ----------------- */
// ARMING_SEQUENCE
#define ARMING_SEQUENCE_LOW_THROTTLE_REQUIRED		0
#define ARMING_SEQUENCE_HIGH_THROTTLE_REQUIRED		1
#define ARMING_SEQUENCE_LOW_THROTTLE_REQUIRED_AGAIN 2
#define ARMING_SEQUENCE_ARMED						3

#define ARMING_SEQUENCE_IS_DISABLED					0

// THROTTLE CALIBRATION
/* use different values of motor speed if the quad ascends / descends too quickly with a centered throttle */
#define MOTOR_SPEED_REQUIRED_FOR_LIFT				1100.0

/* use different values of sensitivity if the quad ascends / descends too quickly when changing the throttle.
 * this is used in a calculation MOTOR_SPEED + (SENS. * THROTTLE) where THROTTLE is in the range -50 : +50.
 */
#define THROTTLE_SENSITIVITY						10.0

// PID
/* any algorithm PID calculations outside these bounds will be reduced to the bound value */
#define PID_MINIMUM_BOUND							-100.0
#define PID_MAXIMUM_BOUND							100.0


// MOTOR SPEED
#define MINIMUM_MOTOR_SPEED							1000
#define MAXIMUM_MOTOR_SPEED							2000


// ANGULAR POSITION
#define HOW_MUCH_I_TRUST_THE_GYROSCOPE     			0.98f
#define HOW_MUCH_I_TRUST_THE_ACCELEROMETER			1 - HOW_MUCH_I_TRUST_THE_GYROSCOPE
#define GYROSCOPE_SAMPLE_RATE              			1.0f / (1000.0f / 1.0f)	/* sample rate is closer to 1.74k per second now that time is not measured */


// ANALYTICS
#define ANALYTICS_FLUSH_FREQUENCY 		   			1000 / 200 	/* how often to flush the metrics (20 times per second) */
#define	ANALYTICS_CHARACTERS_TO_SEND_PER_FLUSH 		5

// METRICS
#define METRIC_SECONDS_ELAPSED								0
#define METRIC_LOOP_FREQUENCY								1
#define METRIC_GYROSCOPE_X_POSITION							2
#define METRIC_GYROSCOPE_Y_POSITION							3
#define METRIC_GYROSCOPE_Z_POSITION							4
#define METRIC_GYROSCOPE_TEMPERATURE						5
#define METRIC_GYROSCOPE_SAMPLE_RATE						6
#define METRIC_ACCELEROMETER_X_POSITION						7
#define METRIC_ACCELEROMETER_Y_POSITION						8
#define METRIC_ACCELEROMETER_Z_POSITION						9
#define METRIC_ACCELEROMETER_SAMPLE_RATE					10
#define METRIC_ANGULAR_X_POSITION							11
#define METRIC_ANGULAR_Y_POSITION							12
#define METRIC_ANGULAR_Z_POSITION							13
#define METRIC_PID_X_ADJUSTMENT								14
#define METRIC_PID_Y_ADJUSTMENT								15
#define METRIC_REMOTE_PID_PROPORTIONAL						16
#define METRIC_REMOTE_PID_INTEGRAL							27
#define METRIC_REMOTE_THROTTLE								17
#define METRIC_PROPELLOR_B_SPEED							18
#define METRIC_PROPELLOR_E_SPEED							19
#define METRIC_PROPELLOR_C_SPEED							20
#define METRIC_PROPELLOR_A_SPEED							21
#define METRIC_METRICS_BUFFER_SIZE							22
#define METRIC_DEBUG_VALUE_1								23
#define METRIC_DEBUG_VALUE_2								24
#define METRIC_DEBUG_VALUE_3								25
#define METRIC_DEBUG_VALUE_4								26

#define YELLOW_LED GPIO_Pin_12
#define ORANGE_LED GPIO_Pin_13
#define RED_LED    GPIO_Pin_14
#define BLUE_LED   GPIO_Pin_15

extern uint8_t data_receive[10];
/* -------------------- Accelerometer Variable ----------------- */
extern bool isReadingAccelerometer;


typedef struct AccelerometerReading {
	float x;
	float y;
	float z;
	float xG;
	float yG;
	float zG;
	float xOffset;
	float yOffset;
	float zOffset;
	uint32_t readings;
}AccelerometerReading;

extern struct AccelerometerReading accelerometerReading;

/* -------------------- Analytics Variable ----------------- */

// Variable size of buffer ring
#define RING_BUFFER_SIZE 1024

// Structure to store data
typedef struct RingBuffer
{
	uint16_t buffer[RING_BUFFER_SIZE];
	int head;
	int tail;
	int count;
} RingBuffer;
extern RingBuffer metricsRingBuffer;


/* -------------------- Angular_Pos Variable ----------------- */
extern bool sensorToggle;
typedef struct AngularPosition {
	float x;
	float y;
	float z;
}AngularPosition;

extern struct AngularPosition angularPosition;

/* -------------------- Gyroscope Variable ----------------- */
extern bool isReadingGyroscope;

typedef struct GyroscopeReading {
	float x;
	float y;
	float z;
	float gyroscopeTemperature;
	float xOffset;
	float yOffset;
	float zOffset;
	uint32_t readings;
}GyroscopeReading;

extern struct GyroscopeReading gyroscopeReading;

/* -------------------- MAGNETOMETER Variable ----------------- */
typedef struct MagnetometerReading {
	int16_t x;
	int16_t y;
	int16_t z;
}MagnetometerReading;

extern struct MagnetometerReading magnetometerReading;

/* -------------------- panic Variable ----------------- */
extern uint32_t clearWarningsOnSecondsElapsed;

/* -------------------- PID Variable ----------------- */
/*
 * Proportional: relating to the present error
 * Integral: relating to the error over time
 * Derivative: relating to the prediction of future errors (based on the current rate of change)
 */
typedef struct Pid {
	float proportional;
	float integral;
	float differential;
	float lastError;
	float cumulativeError;
}Pid;

/* -------------------- PWM Variable ----------------- */
// Function pointer to get some info from channel
typedef void (*DutyCycleModifier)(float);
typedef void (*DutyCycleSetter)(float);
typedef float (*DutyCycleReader)();

extern float channel1Pulse;
extern float channel2Pulse;
extern float channel3Pulse;
extern float channel4Pulse;

// New structure to set return value
typedef struct DutyCycle {
	DutyCycleSetter set;
    DutyCycleModifier update;
    DutyCycleReader get;
}DutyCycle;

/* -------------------- pwm_input Variable ----------------- */
// Structure data to manage PWM input
typedef struct PWMInput {
    volatile float dutyCycle;
    volatile float frequency;
    uint8_t hclckDivisor;

}PWMInput;

// Initialize 4 PWM input on board
extern struct PWMInput pwmInputTimer4;
extern struct PWMInput pwmInputTimer5;
extern struct PWMInput pwmInputTimer9;
extern struct PWMInput pwmInputTimer12;

/* -------------------- Remote control Variable ----------------- */
// Store throttle
extern struct PWMInput* throttle;

extern struct PWMInput* remotePidProportional;

extern struct PWMInput* remotePidIntegral;

extern struct PWMInput* resetAngularPosition;

/* -------------------- Timer 2 Variable ----------------- */
extern uint32_t secondsElapsed;
extern uint32_t intermediateMillis;


#endif /* INC_CONFIG_TYPES_VARS_H_ */
