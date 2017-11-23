/*
 * gpio.c
 *
 *  Created on: Sep 26, 2017
 *      Author: Matthew
 */

#include "comp494.h"
#include "gpio.h"

void GpioInit(void)
{
	// PA5 -> Green LED (active high)
	// Initialize PA5 as an output and initialize LED to off
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
	// PC13 <- Blue pushbutton (active low)
	// Initialize PC13 as an input
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_INPUT);
}
