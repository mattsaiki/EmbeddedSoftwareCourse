/**
  ******************************************************************************
  * main.c
  * Matt Saiki
  * Session 6 - Morse Code Headlight
  *
  ******************************************************************************
*/


#include "comp494.h"
#include "gpio.h"
#include "morse.h"
			
#define DLY_CNT_TICK		10000

int main(void)
{
	int TickDlyCnt;

	//Initialize the hardware, turn on the LED
	GpioInit();
	GPIO_LED_ENA();

	//Wait for the button to be pressed
	while(!GPIO_PB_IS_PRESSED())
		;

	//Turn off the LED
	GPIO_LED_DIS();

	// Initialize the Morse Code state machine with the string to be sent
	// Passing the address of the string
	// Char * in init
	MorseStateMachineInit("USD 2017");

	//Main loop
	while(TRUE)
	{
		// Pace the tick timer
		for(TickDlyCnt = 0; TickDlyCnt < DLY_CNT_TICK; TickDlyCnt++)
			;

		// Clock the state machine
		MorseStateMachineClock();

		//If the string has been sent, resend it
		if (MorseStateMachineIsIdle())
		{
			MorseStateMachineInit("USD 2017");
		}
	}
}
