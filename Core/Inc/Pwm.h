/*
 * Pwm.h
 *
 *  Created on: Mar 26, 2020
 *      Author: PC
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "main.h"
#include "Config_Types_Vars.h"

// Initialize PWM
extern void InitialisePWM(void);
// Set pulse channel for PWM
extern void SetTim3Channel1(float pulse);
extern void SetTim3Channel2(float pulse);
extern void SetTim3Channel3(float pulse);
extern void SetTim3Channel4(float pulse);
// Function to update pulse for pwm
extern void UpdateTim3Channel1(float pulse);
extern void UpdateTim3Channel2(float pulse);
extern void UpdateTim3Channel3(float pulse);
extern void UpdateTim3Channel4(float pulse);
// Function to get value of pulse
extern float ReadTim3Channel1Pulse();
extern float ReadTim3Channel2Pulse();
extern float ReadTim3Channel3Pulse();
extern float ReadTim3Channel4Pulse();
// Function initialize channel
extern DutyCycle InitialisePWMChannel(uint8_t channel);


#endif /* INC_PWM_H_ */
