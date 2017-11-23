//
// COMP 494 - Tom Lupfer
//
// GPIO module
//

#include "comp494.h"
#include "gpio.h"


void GpioInit(void)
{
	// Enable the clock for the PAx GPIO pins
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

	// PA4 -> debug digital output
	// Initialize PA4 as an output and initialize to low
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);

	// PA5 -> Green LED (active high)
	// Initialize PA5 as an output and initialize LED to off
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);

	// Enable the clock for the PCx GPIO pins
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

	// PC13 <- Blue Pushbutton (active low)
	// Initialize PC13 as an input
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_INPUT);
}
