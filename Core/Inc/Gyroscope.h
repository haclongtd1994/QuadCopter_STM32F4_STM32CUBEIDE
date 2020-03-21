/*
 * Gyroscope.h
 *
 *  Created on: Mar 15, 2020
 *      Author: PC
 */

#ifndef INC_GYROSCOPE_H_
#define INC_GYROSCOPE_H_

#include "main.h"
#include "Config_Types_Vars.h"

void i2c_write_gyroscope(uint8_t reg, uint8_t value);
void i2c_read_gyroscope(uint8_t reg, uint8_t numberofbytes);
extern void InitialiseGyroscope();
extern void ReadGyroscope();

#endif /* INC_GYROSCOPE_H_ */
