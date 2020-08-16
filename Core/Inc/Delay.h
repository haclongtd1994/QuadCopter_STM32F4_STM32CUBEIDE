/*
 * Delay.h
 *
 *  Created on: Mar 15, 2020
 *      Author: PC
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "main.h"
#include "Config_Types_Vars.h"

extern void EnableTiming(void);					// Function to enable delay timer
extern void TimingDelay(unsigned int tick);	// Function delay with cycle of chip
extern void WaitASecond(void);						// Function delay 1 second
extern void WaitAMillisecond(void);				// Function delay 1 milisecond
extern void WaitAFewMillis(int16_t millis);	// Function delay miliseconds

#endif /* INC_DELAY_H_ */
