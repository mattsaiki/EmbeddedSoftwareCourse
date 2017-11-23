//
// COMP 494 - Tom Lupfer
//
// LED module
//

#include "comp494.h"
#include "gpio.h"
#include "led.h"

#define LED_BLINK_ST_OFF	0
#define LED_BLINK_ST_ON		1
#define LED_BLINK_ST_IDL	2

#define LED_ON_TICKS		100
#define LED_OFF_TICKS		400

static int LedOnDelay = LED_ON_TICKS;
static int LedOffDelay = LED_OFF_TICKS;

static int LedBlinkState = LED_BLINK_ST_ON;

void LedBlinkSMClock(void)
{
	static int LedBlinkDelay = 400;

	switch (LedBlinkState)
	{
	case LED_BLINK_ST_OFF:
		if (--LedBlinkDelay == 0)
		{
			GPIO_LED_ENA();
			LedBlinkDelay = LedOnDelay;
			LedBlinkState = LED_BLINK_ST_ON;
		}
		break;

	case LED_BLINK_ST_ON:
		if (--LedBlinkDelay == 0)
		{
			GPIO_LED_DIS();
			LedBlinkDelay = LedOffDelay;
			LedBlinkState = LED_BLINK_ST_OFF;
		}
		break;
	case LED_BLINK_ST_IDL:
		GPIO_LED_DIS();
		break;
	}
}

int GetState()
{
	return LedBlinkState;
}

void SetState(int State)
{
	LedBlinkState = State;
}

int GetOnPeriod()
{
	return LedOnDelay;
}

void SetOnPeriod(int Period)
{
	LedOnDelay = Period;
}

int GetOffPeriod()
{
	return LedOffDelay;
}

void SetOffPeriod(int Period)
{
	LedOffDelay = Period;
}
