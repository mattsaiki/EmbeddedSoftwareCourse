/*
 * led.c
 *
 *  Created on: Oct 3, 2017
 *      Author: Matthew
 */

#include "comp494.h"
#include "gpio.h"
#include "led.h"
#include "stdio.h"
#include "eprintf.h"

#define LED_BLINK_ST_OFF		0
#define LED_BLINK_ST_ON			1

#define LED_ON_TICKS			100
#define LED_OFF_TICKS			400

void LedBlinkSMClock(void)
{
	static int LedBlinkState = LED_BLINK_ST_OFF;
	static int LedBlinkDelay = LED_OFF_TICKS;

	switch(LedBlinkState)
	{
	case LED_BLINK_ST_OFF:
		if(--LedBlinkDelay == 0)
		{
			GPIO_LED_ENA();
			LedBlinkDelay = LED_ON_TICKS;
			LedBlinkState = LED_BLINK_ST_ON;
		}
		break;

	case LED_BLINK_ST_ON:

		if(--LedBlinkDelay == 0)
		{
			eprintf("Hello, world! It is %5%\n", 10);
			GPIO_LED_DIS();
			LedBlinkDelay = LED_OFF_TICKS;
			LedBlinkState = LED_BLINK_ST_OFF;
		}
		break;
	}
}
