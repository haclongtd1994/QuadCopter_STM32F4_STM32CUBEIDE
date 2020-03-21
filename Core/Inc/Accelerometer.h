/*
 * Accelerometer.h
 *
 *  Created on: Mar 14, 2020
 *      Author: PC
 */

#ifndef INC_ACCELEROMETER_H_
#define INC_ACCELEROMETER_H_

#include "main.h"
#include "Config_Types_Vars.h"

void i2c_write_accelerometer(uint8_t reg, uint8_t value);
void i2c_read_accelerometer(uint8_t reg, uint8_t numberofbytes);
extern void InitialiseAccelerometer();
extern void ReadAccelerometer();

#endif /* INC_ACCELEROMETER_H_ */
