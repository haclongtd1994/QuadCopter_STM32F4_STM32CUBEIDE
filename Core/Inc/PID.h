/*
 * PID.h
 *
 *  Created on: Sep 6, 2020
 *      Author: PC
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"
#include "Config_Types_Vars.h"

// Function to initialize pid
struct Pid InitialisePid(float proportional, float integral, float differential);

// Calculate PID Adj
float CalculatePidAdjustment(Pid* pid, float current, float target);

#endif /* INC_PID_H_ */
