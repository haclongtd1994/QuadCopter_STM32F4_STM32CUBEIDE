/*
 * Pwm.c
 *
 *  Created on: Mar 26, 2020
 *      Author: PC
 */
#include "main.h"
#include "Pwm.h"
#include "math.h"

TIM_HandleTypeDef htim3;

/******************************************************************************
* Function Name: InitialisePWM
* Description  : This function initialize pwm.
* Arguments    : None
* Return Value : None
******************************************************************************/
void InitialisePWM()
{
	/* set all of the pulse values to 0% */
	channel1Pulse = 1000;
	channel2Pulse = 1000;
	channel3Pulse = 1000;
	channel4Pulse = 1000;
	// Initial for all channel of timer 3 using PWM
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);

    HAL_TIM_PWM_Init(&htim3);

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (uint32_t) 700);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (uint32_t) 700);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, (uint32_t) 700);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, (uint32_t) 700);
}

/******************************************************************************
* Function Name: SetTim3Channel1
* Description  : This function set pulse to channel 1.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
void SetTim3Channel1(float pulse) {
	if (pulse >= 1000 && pulse <= 2000) {
		channel1Pulse = pulse;				// Update new data pulse of channel 1
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (uint32_t) pulse);		// Update value of capture/compare register 1
	}
}

/******************************************************************************
* Function Name: SetTim3Channel2
* Description  : This function set pulse to channel 2.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
void SetTim3Channel2(float pulse) {
	if (pulse >= 1000 && pulse <= 2000) {
		channel2Pulse = pulse;
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (uint32_t) pulse);
	}
}

/******************************************************************************
* Function Name: SetTim3Channel3
* Description  : This function set pulse to channel 3.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
void SetTim3Channel3(float pulse) {
	if (pulse >= 1000 && pulse <= 2000) {
		channel3Pulse = pulse;
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, (uint32_t) pulse);
	}
}

/******************************************************************************
* Function Name: SetTim3Channel4
* Description  : This function set pulse to channel 4.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
void SetTim3Channel4(float pulse) {
	if (pulse >= 1000 && pulse <= 2000) {
		channel4Pulse = pulse;
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, (uint32_t) pulse);
	}
}

/******************************************************************************
* Function Name: UpdateTim3Channel1
* Description  : This function update pulse of channel 1.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
void UpdateTim3Channel1(float pulse) {
	SetTim3Channel1(channel1Pulse + pulse);
}

/******************************************************************************
* Function Name: UpdateTim3Channel2
* Description  : This function update pulse of channel 2.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
void UpdateTim3Channel2(float pulse) {
	SetTim3Channel2(channel2Pulse + pulse);
}

/******************************************************************************
* Function Name: UpdateTim3Channel3
* Description  : This function update pulse of channel 3.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
void UpdateTim3Channel3(float pulse) {
	SetTim3Channel3(channel3Pulse + pulse);
}

/******************************************************************************
* Function Name: UpdateTim3Channel4
* Description  : This function update pulse of channel 4.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
void UpdateTim3Channel4(float pulse) {
	SetTim3Channel4(channel4Pulse + pulse);
}

/******************************************************************************
* Function Name: ReadTim3Channel1Pulse
* Description  : This function read current value pulse channel 1.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
float ReadTim3Channel1Pulse() {
	return channel1Pulse;
}

/******************************************************************************
* Function Name: ReadTim3Channel2Pulse
* Description  : This function read current value pulse channel 2.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
float ReadTim3Channel2Pulse() {
	return channel2Pulse;
}

/******************************************************************************
* Function Name: ReadTim3Channel3Pulse
* Description  : This function read current value pulse channel 3.
* Arguments    : Pulse
* Return Value : None
******************************************************************************/
float ReadTim3Channel3Pulse() {
	return channel3Pulse;
}

/******************************************************************************
* Function Name: ReadTim3Channel4Pulse
* Description  : This function read current value pulse channel 4.
* Arguments    : Pulse
* Return Value : None

******************************************************************************/
float ReadTim3Channel4Pulse() {
	return channel4Pulse;
}

/******************************************************************************
* Function Name: InitialisePWMChannel
* Description  : This function to initialize pwm channel.
* Arguments    : GPIO, pin, pinsource, channel.
* Return Value : Structure DutyCycle.
******************************************************************************/
DutyCycle InitialisePWMChannel(uint8_t channel)
{
    struct DutyCycle dutyCycle;

    if (channel == 1) {
    	dutyCycle.set = &SetTim3Channel1;
        dutyCycle.update = &UpdateTim3Channel1;
        dutyCycle.get = &ReadTim3Channel1Pulse;
    } else if (channel == 2) {
    	dutyCycle.set = &SetTim3Channel2;
        dutyCycle.update = &UpdateTim3Channel2;
        dutyCycle.get = &ReadTim3Channel2Pulse;
    } else if (channel == 3) {
    	dutyCycle.set = &SetTim3Channel3;
        dutyCycle.update = &UpdateTim3Channel3;
        dutyCycle.get = &ReadTim3Channel3Pulse;
    } else if (channel == 4) {
    	dutyCycle.set = &SetTim3Channel4;
        dutyCycle.update = &UpdateTim3Channel4;
        dutyCycle.get = &ReadTim3Channel4Pulse;
    }

    // note that this will be copied - probably better off using a reference.
    return dutyCycle;
}
