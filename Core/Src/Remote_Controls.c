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
	* Channel 3 on the RC receiver
	*/
	//PB.06
	throttle = &pwmInputTimer4;
	throttle->hclckDivisor = 2.0f;
	throttle->dutyCycle = 0.0;
	throttle->frequency = 0.0;

	/* rudder: spin to the left or right on a flat plane
	* Channel 4 on the RC receiver
	*/
	//PA.00
	remotePidProportional = &pwmInputTimer5;
	remotePidProportional->hclckDivisor = 2.0f;
	remotePidProportional->dutyCycle = 0.0;
	remotePidProportional->frequency = 0.0;

	/* Aileron: fly sideways left or right
	* Channel 1 on the RC receiver
	*/
	//PE.05
	remotePidIntegral = &pwmInputTimer9;
	remotePidIntegral->hclckDivisor = 2.0f;
	remotePidIntegral->dutyCycle = 0.0;
	remotePidIntegral->frequency = 0.0;

	/* elevator: fly forwards or backwards
	* Channel 2 on the RC receiver
	*/
	//PB.14
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
float CalculatePercentageOfMaximum(float dutyCycle, float frequency, \
  float max_num, float min_num) {
	/* how can I tell if something is NAN? */

	/*A duty cycle of 2ms is on for max_num% of the time @ 71Hz(13.8ms period)*/
	float maximum = (max_num / (1000 / frequency)) * 100;
	/*A duty cycle of 1ms is on for min_num% of the time @ 71Hz(13.8ms period)*/
	float minimum = (min_num / (1000 / frequency)) * 100;
	if (dutyCycle > maximum){
		return 100.0;
	}
	else if (dutyCycle < minimum){
		return 0;
	}
	else{
		float percentageOn = \
		  ((dutyCycle - minimum) / (maximum - minimum)) * 100.0;
		if (percentageOn > 100.0) {
			return 100.0;
		}
		else if (percentageOn < 0.0) {
			return 0.0;
		}
		return percentageOn;
	}
}

/******************************************************************************
* Function Name: ReadRemoteThrottle
* Description  : This function calculate percentage of throttle.
* Arguments    : None
* Return Value : Float
******************************************************************************/
float ReadRemoteThrottle() {
	// Freq = 71, T = (1/71)*1000 = 14.084 ms
	// Max_num = T * max_duty% = (14.084 * 13.4)/100 = 1.88
	// Min_num = T * min_duty% = (14.084 * 6.325)/100 = 0.89
	return CalculatePercentageOfMaximum(throttle->dutyCycle, \
	  throttle->frequency, 1.88, 0.89);
}

/******************************************************************************
* Function Name: ReadRemotePidProportional
* Description  : This function calculate percentage of PID proportional.
* Arguments    : None
* Return Value : Float
******************************************************************************/
float ReadRemotePidProportional() {
	// Freq = 71, T = (1/71)*1000 = 14.084 ms
	// Max_num = T * max_duty% = (14.084 * 12.1)/100 = 1.7
	// Min_num = T * min_duty% = (14.084 * 8.54)/100 = 1.21
	return CalculatePercentageOfMaximum(remotePidProportional->dutyCycle, \
	  remotePidProportional->frequency, 1.7, 1.21);
}

/******************************************************************************
* Function Name: ReadRemotePidIntegral
* Description  : This function calculate percentage of PID Integral.
* Arguments    : None
* Return Value : Float
******************************************************************************/
float ReadRemotePidIntegral() {
	// Freq = 70, T = (1/70)*1000 =  14.286ms
	// Max_num = T * max_duty% = (14.286 * 12)/100 = 1.71
	// Min_num = T * min_duty% = (14.286 * 8.5)/100 = 1.22
	return CalculatePercentageOfMaximum(remotePidIntegral->dutyCycle, \
	  remotePidIntegral->frequency, 1.71, 1.22);
}

/******************************************************************************
* Function Name: ReadResetAngularPosition
* Description  : This function calculate percentage of position of angular.
* Arguments    : None
* Return Value : Float
******************************************************************************/
float ReadResetAngularPosition() {
	// Freq = 71, T = (1/71)*1000 = 14.084 ms
	// Max_num = T * max_duty% = (14.084 * 12)/100 = 1.69
	// Min_num = T * min_duty% = (14.084 * 8.46)/100 = 1.2
	return CalculatePercentageOfMaximum(resetAngularPosition->dutyCycle, \
	  resetAngularPosition->frequency, 1.69, 1.2);
}
