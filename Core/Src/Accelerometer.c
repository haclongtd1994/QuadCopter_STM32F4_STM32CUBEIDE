/*
 * Accelerometer.c
 *  Device: ADXL345
 *  Created on: Mar 14, 2020
 *      Author: PC
 */

#include "Accelerometer.h"
#include "math.h"

I2C_HandleTypeDef hi2c1;

/******************************************************************************
* Function Name: i2c_write_accelerometer
* Description  : This function write to register of slave device Accelerometer 0x53: value.
* Arguments    : Device address, Register of device, Value.
* Return Value : None
******************************************************************************/
void i2c_write_accelerometer(uint8_t reg, uint8_t value)
{
	uint8_t data[2]={0};
	data[0] = reg;
	data[1] = value;
	HAL_I2C_Master_Transmit (&hi2c1, 0x53<<1, data, 2, 10);
}
/******************************************************************************
* Function Name: i2c_read_accelerometer
* Description  : This function read from register of slave device Accelerometer 0x53: value.
* Arguments    : Device address, Register of device, Value.
* Return Value : None
******************************************************************************/
void i2c_read_accelerometer(uint8_t reg, uint8_t numberofbytes)
{
	HAL_I2C_Mem_Read(&hi2c1, 0x53<<1, reg, I2C_MEMADD_SIZE_8BIT, data_receive, numberofbytes, 100);
}
/******************************************************************************
* Function Name: InitialiseAccelerometer
* Description  : This function initialize Accelerometer.
* Arguments    : None
* Return Value : None
******************************************************************************/
void InitialiseAccelerometer()
{
	/* Turn off inactivity / activity interrupts for all axis */
	i2c_write_accelerometer(0x27, 0x0);

	/* Turn off interrupts for tap detection */
	i2c_write_accelerometer(0x2A, 0x0);

	/* Select "Normal" power, this uses more power and has less noise (0x0.)
	 * Select output data rate of 100kHz (0x.A)
	 * Output data rate should match the bus frequency / sample rate. To make it
	 *   400kHz, change this value to (0x.C)
	 */
	i2c_write_accelerometer(0x2C, 0x0A);

	/* Put the accelerometer in "measurement" mode
	 * Ensure the device will not sleep */
	i2c_write_accelerometer(0x2D, 0x08);

	/* Turn off all interrupts outputs */
	i2c_write_accelerometer(0x2E, 0x0);

	/* Turn self test off.
	 * Using three wire SPI mode (i2c?).
	 * Left justify mode (most significant bit).
	 * Full resolution, full range.
	 */
	i2c_write_accelerometer(0x31, 0x4F);

	/* Initialize the accelerometer at zero position */
	accelerometerReading.x = 0.0f;
	accelerometerReading.y = 0.0f;
	accelerometerReading.z = 0.0f;
	accelerometerReading.xOffset = 0.0f;
	accelerometerReading.yOffset = 0.0f;
	accelerometerReading.zOffset = 0.0f;
	accelerometerReading.readings = 0;

	isReadingAccelerometer = 0;
}
/******************************************************************************
* Function Name: ReadAccelerometer
* Description  : This function read data and store to struct variable.
* Arguments    : None
* Return Value : None
******************************************************************************/
void ReadAccelerometer()
{
	// Check data exist or not by global variable bool isReadingAccelerometer
	if (!isReadingAccelerometer) {
		/* kick off a new read of the accelerometer values with: \
			device ADXL345 (0x53) and pheripheral (0x32), Numbyte want read (6) */
		i2c_read_accelerometer(0x32, 6);
		isReadingAccelerometer = 1;
		return;
	}

	// done! convert the values to a reading
	uint8_t xLow = data_receive[0];
	uint8_t xHigh = data_receive[1];
	uint8_t yLow = data_receive[2];
	uint8_t yHigh = data_receive[3];
	uint8_t zLow = data_receive[4];
	uint8_t zHigh = data_receive[5];

	int16_t rawX = (((int16_t) xHigh << 8) | xLow);
	int16_t rawY = (((int16_t) yHigh << 8) | yLow);
	int16_t rawZ = (((int16_t) zHigh << 8) | zLow);

	/* full resolution means that there is a scale of 3.9mG/LSB */
	accelerometerReading.xG = rawX * 0.0039;
	accelerometerReading.yG = rawY * 0.0039;
	accelerometerReading.zG = rawZ * 0.0039;

	float calibratedX = accelerometerReading.xG - accelerometerReading.xOffset;
	float calibratedY = accelerometerReading.yG - accelerometerReading.yOffset;
	float calibratedZ = accelerometerReading.zG - accelerometerReading.zOffset;

	/* calculate the squares */
	float xSquared = calibratedX * calibratedX;
	float ySquared = calibratedY * calibratedY;
	float zSquared = calibratedZ * calibratedZ;

	// Simulation change to degrees of X, Y. We calculation from vector of both with Z
	float xInDegrees = atan(calibratedY / sqrt(xSquared + zSquared)) * 180.0f / 3.141592f;
	float yInDegrees = atan(calibratedX / sqrt(ySquared + zSquared)) * 180.0f / 3.141592f;
	float zInDegress = 0.0f; // for now.

	if (!isnan(xInDegrees) && !isnan(yInDegrees) && !isnan(zInDegress)) {
		accelerometerReading.x = xInDegrees;
		accelerometerReading.y = yInDegrees;
		accelerometerReading.z = zInDegress;
		accelerometerReading.readings++;
	}

	isReadingAccelerometer = 0;
}
