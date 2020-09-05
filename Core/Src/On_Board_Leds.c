/*
 * On_Board_Leds.c
 *
 *  Created on: Sep 5, 2020
 *      Author: PC
 */
#include "On_Board_Leds.h"

/******************************************************************************
* Function Name: TurnOn
* Description  : This function turn on led on board.
* Arguments    : None
* Return Value : None
******************************************************************************/
void TurnOn(uint16_t leds) {
	HAL_GPIO_WritePin(GPIOD, leds, GPIO_PIN_SET);
}

/******************************************************************************
* Function Name: TurnOff
* Description  : This function turn on led off board.
* Arguments    : None
* Return Value : None
******************************************************************************/
void TurnOff(uint16_t leds) {
	HAL_GPIO_WritePin(GPIOD, leds, GPIO_PIN_RESET);
}

