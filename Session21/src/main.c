//
// COMP 494 - Tom Lupfer
//
// Session 21 - Interrupt-driven full-duplex UART communications
//

#include "comp494.h"
#include "clock.h"
#include "gpio.h"
#include "led.h"
#include "uart.h"
#include "cmd.h"


int main(void)
{
	// Initialize hardware
	ClockInit();
	GpioInit();
	UartInit();
	GPIO_LED_ENA();

	// Main loop
	while (TRUE)
	{
		// Wait for the 1msec tick timer
		ClockWaitForTick();
		// Clock the state machines
		LedBlinkSMClock();
		CmdProcessSMClock();
	} // End - main loop

}
