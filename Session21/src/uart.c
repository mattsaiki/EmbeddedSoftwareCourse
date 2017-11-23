//
// COMP 494 - Tom Lupfer
//
// UART module
//

#include "comp494.h"
#include "uart.h"


#define	UART_RCV_BUFSIZE		600				// max number of chars + 1 in UART receive buffer
#define	UART_XMT_BUFSIZE		600				// max number of chars + 1 in UART transmit buffer


static char		UartRcvBuf[UART_RCV_BUFSIZE];	// UART receive buffer
static int		UartRcvBufHeadIdx;				// UART receive buffer head index
static int		UartRcvBufTailIdx;				// UART receive buffer tail index

static char		UartXmtBuf[UART_XMT_BUFSIZE];	// UART transmit buffer
static int		UartXmtBufHeadIdx;				// UART receive buffer head index
static int		UartXmtBufTailIdx;				// UART receive buffer tail index


void UartInit(void)
{
	// Reset the receive and transmit buffer head, tail indexes
	UartRcvBufHeadIdx = 0;
	UartRcvBufTailIdx = 0;
	UartXmtBufHeadIdx = 0;
	UartXmtBufTailIdx = 0;

	// (1) Enable GPIO clock and configures the USART pins

	// Enable the peripheral clock of GPIO Port
	USARTx_GPIO_CLK_ENABLE();

	// Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up
	LL_GPIO_SetPinMode(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_MODE_ALTERNATE);
	USARTx_SET_TX_GPIO_AF();
	LL_GPIO_SetPinSpeed(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_PULL_UP);

	// Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up
	LL_GPIO_SetPinMode(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_MODE_ALTERNATE);
	USARTx_SET_RX_GPIO_AF();
	LL_GPIO_SetPinSpeed(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_PULL_UP);

	// (2) NVIC Configuration for USART interrupts

	// Set priority for USARTx_IRQn
	// Enable USARTx_IRQn
	NVIC_SetPriority(USARTx_IRQn, 0);
	NVIC_EnableIRQ(USARTx_IRQn);

	// (3) Enable USART peripheral clock and clock source
	USARTx_CLK_ENABLE();

	// (4) Configure USART functional parameters

	// Disable USART prior modifying configuration registers
	// Note: Commented as corresponding to Reset value
	// LL_USART_Disable(USARTx_INSTANCE);

	// TX/RX direction
	LL_USART_SetTransferDirection(USARTx_INSTANCE, LL_USART_DIRECTION_TX_RX);

	// 8 data bit, 1 start bit, 1 stop bit, no parity
	LL_USART_ConfigCharacter(USARTx_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

	// No Hardware Flow control
	// Reset value is LL_USART_HWCONTROL_NONE
	// LL_USART_SetHWFlowCtrl(USARTx_INSTANCE, LL_USART_HWCONTROL_NONE);

	// Oversampling by 16
	// Reset value is LL_USART_OVERSAMPLING_16
	// LL_USART_SetOverSampling(USARTx_INSTANCE, LL_USART_OVERSAMPLING_16);

	// Set Baudrate to 115200 using APB frequency set to 32000000 Hz
	// Frequency available for USART peripheral can also be calculated through LL RCC macro
	// Ex :
   	// 	   Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance
   	// 	   In this example, Peripheral Clock is expected to be equal to 32000000 Hz => equal to SystemCoreClock
	LL_USART_SetBaudRate(USARTx_INSTANCE, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200);

	// (5) Enable USART
	LL_USART_Enable(USARTx_INSTANCE);

	// Enable RXNE interrupts
	LL_USART_EnableIT_RXNE(USARTx_INSTANCE);
}


Bool UartCharAvail(void)
{
	// Return the receive buffer not empty status
	return (UartRcvBufHeadIdx != UartRcvBufTailIdx);
}


char UartCharRead(void)
{
	char	RetChar;	// char to be returned

	// If the receive buffer is empty, return the null char
	if (UartRcvBufHeadIdx == UartRcvBufTailIdx)
	{
		return '\0';
	}

	// Read the value from the receive buffer, incr/wrap the head index
	RetChar = UartRcvBuf[UartRcvBufHeadIdx];
	if (++UartRcvBufHeadIdx == UART_RCV_BUFSIZE)
	{
		UartRcvBufHeadIdx = 0;
	}

	return RetChar;
}


void UartCharWrite(char OutChar)
{
	// Check that the transmit buffer is not full, otherwise throw away the char
	if ((!((UartXmtBufHeadIdx == 0) && (UartXmtBufTailIdx == (UART_XMT_BUFSIZE - 1)))) &&
		(!(UartXmtBufTailIdx == (UartXmtBufHeadIdx - 1))))
	{
		// Store the char in the transmit buffer, incr/wrap the tail index
		UartXmtBuf[UartXmtBufTailIdx] = OutChar;
		if (++UartXmtBufTailIdx == UART_XMT_BUFSIZE)
		{
			UartXmtBufTailIdx = 0;
		}
		// Enable the transmit data register empty interrupt
		  LL_USART_EnableIT_TXE(USARTx_INSTANCE);
	}
}


void UartIrqHandler(void)
{
	// Receive handler
	if (LL_USART_IsActiveFlag_RXNE(USARTx_INSTANCE))
	{
		// Read the char and store it at the current receive buffer tail index
		UartRcvBuf[UartRcvBufTailIdx] = LL_USART_ReceiveData8(USARTx_INSTANCE);

		// If the receive buffer is not full, incr/wrap the tail index
		if ((!((UartRcvBufHeadIdx == 0) && (UartRcvBufTailIdx == (UART_RCV_BUFSIZE - 1)))) &&
			(!(UartRcvBufTailIdx == (UartRcvBufHeadIdx - 1))))
		{
			if (++UartRcvBufTailIdx == UART_RCV_BUFSIZE)
			{
				UartRcvBufTailIdx = 0;
			}
		}

		// Otherwise, throw away the char (do nothing)
	}

	// Transmit handler
	if (LL_USART_IsActiveFlag_TXE(USARTx_INSTANCE))
	{
		// Check that the transmit buffer is not empty
		if (UartXmtBufHeadIdx != UartXmtBufTailIdx)
		{
			// Transmit the value from the buffer, incr/wrap the head index
			LL_USART_TransmitData8(USARTx_INSTANCE, UartXmtBuf[UartXmtBufHeadIdx]);
			if (++UartXmtBufHeadIdx == UART_XMT_BUFSIZE)
			{
				UartXmtBufHeadIdx = 0;
			}
		}
		// If the transmit buffer is empty, disable the transmit data register empty interrupt
		if (UartXmtBufHeadIdx == UartXmtBufTailIdx)
		{
			LL_USART_DisableIT_TXE(USARTx_INSTANCE);
		}
	}
}
