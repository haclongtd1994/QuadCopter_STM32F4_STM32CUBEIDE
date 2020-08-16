/*
 * Remote_Controls.c
 *
 *  Created on: Mar 16, 2020
 *      Author: PC
 */

#include "Remote_Controls.h"
#include "math.h"

/******************************************************************************
* Function Name: InitialiseRemoteControls
* Description  : This function initialize remote controls.
* Arguments    : None
* Return Value : None
******************************************************************************/
void InitialiseRemoteControls() {
	/* throttle: all together now! power (collective pitch?)
	* Channel 1 on the RC receiver
	*/
	// channel 2 - PB.06
	throttle = &pwmInputTimer4;
	throttle->hclckDivisor = 2.0f;
	throttle->dutyCycle = 0.0;
	throttle->frequency = 0.0;

	/* rudder: spin to the left or right on a flat plane
	* Channel 4 on the RC receiver
	*/
	// channel 2 - PA.01
	remotePidProportional = &pwmInputTimer5;
	remotePidProportional->hclckDivisor = 2.0f;
	remotePidProportional->dutyCycle = 0.0;
	remotePidProportional->frequency = 0.0;

	/* Aileron: fly sideways left or right
	* Channel 2 on the RC receiver
	*/
	// channel 2 - PE.05
	remotePidIntegral = &pwmInputTimer9;
	remotePidIntegral->hclckDivisor = 2.0f;
	remotePidIntegral->dutyCycle = 0.0;
	remotePidIntegral->frequency = 0.0;

	/* elevator: fly forwards or backwards
	* Channel 3 on the RC receiver
	*/
	// channel 2 - PB.15
	resetAngularPosition = &pwmInputTimer12;
	resetAngularPosition->hclckDivisor = 2.0f;
	resetAngularPosition->dutyCycle = 0.0;
	resetAngularPosition->frequency = 0.0;
}

/******************************************************************************
* Function Name: CalculatePercentageOfMaximum
* Description  : This function calculate percentage.
* Arguments    : Dutycycle, frequency
* Return Value : Float
******************************************************************************/
/* note this makes assumptions about the minimum and maximum of duty cycles */
float CalculatePercentageOfMaximum(float dutyCycle, float frequency) {
	/* how can I tell if something is NAN? */

	/* A duty cycle of 2ms is on for 11% of the time @ 72Hz (13.8ms period) */
	float maximum = 1.77 / (1000 / frequency) * 100;
	/* A duty cycle of 1ms is on for 5.5% of the time @ 72Hz (13.8ms period) */
	float minimum = 0.47 / (1000 / frequency) * 100;

	float percentageOn = (dutyCycle - (maximum - minimum)) / minimum * 100.0;

	if (percentageOn > 100.0) {
		return 100.0;
	}

	if (percentageOn < 0.0) {
		return 0.0;
	}

	return percentageOn;
}

/******************************************************************************
* Function Name: ReadRemoteThrottle
* Description  : This function calculate percentage of throttle.
* Arguments    : None
* Return Value : Float
******************************************************************************/
float ReadRemoteThrottle() {
	return CalculatePercentageOfMaximum(throttle->dutyCycle, throttle->frequency);
}

/******************************************************************************
* Function Name: ReadRemotePidProportional
* Description  : This function calculate percentage of PID proportional.
* Arguments    : None
* Return Value : Float
******************************************************************************/
float ReadRemotePidProportional() {
	return CalculatePercentageOfMaximum(remotePidProportional->dutyCycle, remotePidProportional->frequency);
}

/******************************************************************************
* Function Name: ReadRemotePidIntegral
* Description  : This function calculate percentage of PID Integral.
* Arguments    : None
* Return Value : Float
******************************************************************************/
float ReadRemotePidIntegral() {
	return CalculatePercentageOfMaximum(remotePidIntegral->dutyCycle, remotePidIntegral->frequency);
}

/******************************************************************************
* Function Name: ReadResetAngularPosition
* Description  : This function calculate percentage of position of angular.
* Arguments    : None
* Return Value : Float
******************************************************************************/
float ReadResetAngularPosition() {
	return CalculatePercentageOfMaximum(resetAngularPosition->dutyCycle, resetAngularPosition->frequency);
}
