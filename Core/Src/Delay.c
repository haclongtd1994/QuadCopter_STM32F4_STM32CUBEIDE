/*
 * Delay.c
 *  Detail information about Debug Watchdog Trigger:
 *    https://developer.arm.com/documentation/100166/0001/Data-Watchpoint-and-Trace-Unit/DWT-Programmers--model?search=5eec6e71e24a5e02d07b259a
 *	  https://web.eecs.umich.edu/~prabal/teaching/eecs373-f10/readings/ARMv7-M_ARM.pdf
 *  Created on: Mar 15, 2020
 *      Author: PC
 */

//******************************************************************************

#include <delay.h>

// Register to control watchdog timer
volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)0xE0001004;
// Count cycle
volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000;
// Debug exception and monitor control register
volatile unsigned int *SCB_DEMCR        = (volatile unsigned int *)0xE000EDFC;

/******************************************************************************
* Function Name: EnableTiming
* Description  : This function to enable time of watchdog timer.
* Arguments    : None
* Return Value : None
******************************************************************************/
void EnableTiming(void)
{
    static int enabled = 0;	// Variable to store previous data status of timer
	// Check if not enable will enable to start timer
    if (!enabled)
    {
        *SCB_DEMCR = *SCB_DEMCR | 0x01000000;	// Stop check error
        *DWT_CYCCNT = 0; 						// Reset the counter
        *DWT_CONTROL = *DWT_CONTROL | 1 ; 		// enable the counter

        enabled = 1;
    }
}

/******************************************************************************
* Function Name: TimingDelay
* Description  : This function to delay with cycle of chip.
* Arguments    : Cycle want delay
* Return Value : None
******************************************************************************/
void TimingDelay(unsigned int tick)
{
    unsigned int start, current;

    start = *DWT_CYCCNT;				// Read first data of count timer

    do
    {
        current = *DWT_CYCCNT;			//Read current value
    } while((current - start) < tick);	//Loop to true
}

/******************************************************************************
* Function Name: WaitASecond
* Description  : This function to delay 1 second.
* Arguments    : None
* Return Value : None
******************************************************************************/
void WaitASecond() {
	TimingDelay(168000000);
}

/******************************************************************************
* Function Name: EnableTiming
* Description  : This function to delay 1 ms.
* Arguments    : None
* Return Value : None
******************************************************************************/
void WaitAMillisecond() {
	TimingDelay(168000);
}

/******************************************************************************
* Function Name: EnableTiming
* Description  : This function delay miliseconds.
* Arguments    : None
* Return Value : None
******************************************************************************/
void WaitAFewMillis(int16_t millis) {
	TimingDelay((168000000 / 1000) * millis);
}

