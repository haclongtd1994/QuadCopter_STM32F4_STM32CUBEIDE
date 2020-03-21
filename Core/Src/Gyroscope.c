/*
 * Gyroscope.c
 *
 *  Created on: Mar 15, 2020
 *      Author: PC
 */


/*
 * Accelerometer.c
 *
 *  Created on: Mar 14, 2020
 *      Author: PC
 */
// Device ITG-3205
#include "Gyroscope.h"
#include "Delay.h"
#include "math.h"

I2C_HandleTypeDef hi2c1;

/******************************************************************************
* Function Name: i2c_write_gyroscope
* Description  : This function write to register of slave device Accelerometer 0x69: value.
* Arguments    : Device address, Register of device, Value.
* Return Value : None
******************************************************************************/
void i2c_write_gyroscope(uint8_t reg, uint8_t value)
{
	uint8_t data[2]={0};
	data[0] = reg;
	data[1] = value;
	HAL_I2C_Master_Transmit (&hi2c1, 0x69<<1, data, 2, 10);
}
/******************************************************************************
* Function Name: i2c_read_gyroscope
* Description  : This function read from register of slave device Accelerometer 0x69: value.
* Arguments    : Device address, Register of device, Value.
* Return Value : None
******************************************************************************/
void i2c_read_gyroscope(uint8_t reg, uint8_t numberofbytes)
{
	HAL_I2C_Mem_Read(&hi2c1, 0x69<<1, reg, I2C_MEMADD_SIZE_8BIT, data_receive, numberofbytes, 100);
}
/******************************************************************************
* Function Name: InitialiseAccelerometer
* Description  : This function initialize Accelerometer.
* Arguments    : None
* Return Value : None
******************************************************************************/
void InitialiseGyroscope()
{
	/* Reset the Gyro.
	 * Note that 0x69 is the address of the Gyro on the bus. Pin 9 is logic high
	 */
	i2c_write_gyroscope(0x3E, 0x80);

	/* Setup:
	 * the full scale range of the gyro should be +/-2000 degrees / second
	 * In theory 2000 degrees / second means the quad would be completely rotating 5 times per second! Probably higher than required.
	 * digital low pass filter bandwidth is 5Hz, internal sample rate is 1kHz.
	 * Note: we could adjust the low pass filter in future to see the impact.
	 */
	i2c_write_gyroscope(0x16, 0x1E);

	/* Set the sample rate
	 * Sample rate = internal sample rate / (divider + 1)
	 * Setting divider to 4 to give a sample rate of 200Hz.
	 * The gyro values will update every 5ms.
	 */
	i2c_write_gyroscope(0x15, 0x04);

	/* Set the clock source to PLL with Z Gyro as the reference.
	 * This should be more stable / accurate than an internal oscillator (which would be greatly affected by temperature)
	 * Probably not as good as an external oscillator though.
	 * Accuracy of internal gyro MEMS oscillators are +/- 2% over temperature.
	 */
	i2c_write_gyroscope(0x3E, 0x03);

	/* The gyro takes 50 milliseconds for zero settling */
	WaitAFewMillis(50);

	/* And will take a further (or is this included?) 20ms for register read / write warm up */
	WaitAFewMillis(20);

	/* initialise the gyroscope reading */
	gyroscopeReading.gyroscopeTemperature = 0.0f;
	gyroscopeReading.x = 0.0f;
	gyroscopeReading.y = 0.0f;
	gyroscopeReading.z = 0.0f;
	gyroscopeReading.xOffset = 0.0f;
	gyroscopeReading.yOffset = 0.0f;
	gyroscopeReading.zOffset = 0.0f;
	gyroscopeReading.readings = 0;

	isReadingGyroscope = 0;
}
/******************************************************************************
* Function Name: ReadAccelerometer
* Description  : This function read data and store to struct variable.
* Arguments    : None
* Return Value : None
******************************************************************************/
void ReadGyroscope()
{
	// Check data exist or not by global variable bool isReadingGyroscope
	if (!isReadingGyroscope) {
		// kick off a new read of the gyroscope values
		i2c_write_gyroscope(0x1B, 8);
		isReadingGyroscope = 1;
		return;
	}

	// done! convert the values to a reading
	uint8_t temperatureHigh = data_receive[0];
	uint8_t temperatureLow = data_receive[1];
	uint8_t xHigh = data_receive[2];
	uint8_t xLow = data_receive[3];
	uint8_t yHigh = data_receive[4];
	uint8_t yLow = data_receive[5];
	uint8_t zHigh = data_receive[6];
	uint8_t zLow = data_receive[7];

	/* Temperature offset: -13200 LSB
	 * Temperature sensitivity: 280 LSB / degrees celcius
	 */
	int16_t rawTemperature = (((int16_t) temperatureHigh << 8) | temperatureLow);
	gyroscopeReading.gyroscopeTemperature = 35 + (rawTemperature + 13200) / 280;

	int16_t rawX = (((int16_t) xHigh << 8) | xLow);
	int16_t rawY = (((int16_t) yHigh << 8) | yLow);
	int16_t rawZ = (((int16_t) zHigh << 8) | zLow);

	/* gyro sensitivity: 14.375 LSB / (degrees / second) */
	float xDegreesPerSecond = ((float) rawX / 14.375f) - gyroscopeReading.xOffset;
	float yDegreesPerSecond = ((float) rawY / 14.375f) - gyroscopeReading.yOffset;
	float zDegreesPerSecond = ((float) rawZ / 14.375f) - gyroscopeReading.zOffset;

	if (!isnan(xDegreesPerSecond) && !isnan(yDegreesPerSecond) && !isnan(zDegreesPerSecond)) {
		gyroscopeReading.x = xDegreesPerSecond;
		gyroscopeReading.y = yDegreesPerSecond;
		gyroscopeReading.z = zDegreesPerSecond;
		gyroscopeReading.readings++;
	}

	isReadingGyroscope = 0;
}
