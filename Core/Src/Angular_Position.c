/*
 * Angular_Position.c
 *
 *  Created on: Mar 15, 2020
 *      Author: PC
 */

#include <Angular_Position.h>
#include <Gyroscope.h>
#include <Accelerometer.h>


/******************************************************************************
* Function Name: InitialiseAngularPosition
* Description  : This function initialize Angular position.
* Arguments    : None
* Return Value : None
******************************************************************************/
/* are my angles consistent - radians, or degrees? */
void InitialiseAngularPosition() {
	/* initialise, assume the quad is level */
	angularPosition.x = 0.0;
	angularPosition.y = 0.0;
	angularPosition.z = 0.0;

	sensorToggle = true;

	InitialiseGyroscope();
	InitialiseAccelerometer();
	//InitialiseMagnetometer();
}

/******************************************************************************
* Function Name: ReadAngularPosition
* Description  : This function read value Angular position.
* Arguments    : None
* Return Value : None
******************************************************************************/
void ReadAngularPosition() {
	if (sensorToggle) {
		/* this gryo is dodgy. Sometimes, the last bit in the read sequence never comes back properly, causing errors on the I2C bus. Silly gyro. */
		ReadGyroscope();

		if (!isReadingGyroscope) {
			sensorToggle = 0;
		}
	} else {
		ReadAccelerometer();

		if (!isReadingAccelerometer) {
			sensorToggle = 1;
		}
	}

	// ReadMagnetometer();

//  seems buggered. will introduce the gyro later.
//	angularPosition.x = HOW_MUCH_I_TRUST_THE_GYROSCOPE * (angularPosition.x + (gyroscopeReading.x * GYROSCOPE_SAMPLE_RATE)) + HOW_MUCH_I_TRUST_THE_ACCELEROMETER * accelerometerReading.x;
//	angularPosition.y = HOW_MUCH_I_TRUST_THE_GYROSCOPE * (angularPosition.y + (gyroscopeReading.y * GYROSCOPE_SAMPLE_RATE)) + HOW_MUCH_I_TRUST_THE_ACCELEROMETER * accelerometerReading.y;
//	angularPosition.z = 0;

	angularPosition.x = accelerometerReading.x;
	angularPosition.y = accelerometerReading.y;
	angularPosition.z = accelerometerReading.z;
}

/******************************************************************************
* Function Name: ResetToAngularZeroPosition
* Description  : This function reset value Angular position.
* Arguments    : None
* Return Value : None
******************************************************************************/
void ResetToAngularZeroPosition() {
	angularPosition.x = 0.0;
	angularPosition.y = 0.0;
	angularPosition.z = 0.0;
}
