In this project, I was provided code that implements simultaneous transmit and receive interrupt-driven asynchronous serial communication via USART2. I modified the eprintf() function from Session17 to implement a state machine-based command processor in cmd.c for the commands:
	help --> prints out commands with explanation
	? --> prints out commands with explanation
	blink ena --> enables blinking LED
	blink dis --> disables blinking LED
	blink ? --> prints status of blink (ena or dis)
	blink on <msec_on> --> sets the duration for how long the LED should stay on
	blink on ? --> prints out the duration of the LED's on period
	blink off <msec_off> --> sets the duration for how long the LED should stay off
	blink off ? --> prints out the duration of the LED's off period
