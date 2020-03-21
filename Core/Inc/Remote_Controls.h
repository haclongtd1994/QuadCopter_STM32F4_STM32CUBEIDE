/*
 * Remote_Controls.h
 *
 *  Created on: Mar 16, 2020
 *      Author: PC
 */

#ifndef INC_REMOTE_CONTROLS_H_
#define INC_REMOTE_CONTROLS_H_

#include "main.h"
#include "Config_Types_Vars.h"

extern void InitialiseRemoteControls(void);			// Function to initialize remote controls
/* These will come back as a percentage */
extern float ReadRemoteThrottle(void);					// Read value of throttle
extern float ReadRemotePidProportional(void);			// Read value of PID proportional
extern float ReadRemotePidIntegral(void);				// Read value of PID Integral
extern float ReadResetAngularPosition(void);			// Read value of position of angular at reset


#endif /* INC_REMOTE_CONTROLS_H_ */
