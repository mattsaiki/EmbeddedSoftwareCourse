//
// COMP 494 - Tom Lupfer
//
// UART module header file
//

#ifndef UART_H_
#define UART_H_

#define USARTx_INSTANCE               USART2
#define USARTx_CLK_ENABLE()           LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define USARTx_IRQn                   USART2_IRQn
#define USARTx_IRQHandler             USART2_IRQHandler

#define USARTx_GPIO_CLK_ENABLE()      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#define USARTx_TX_PIN                 LL_GPIO_PIN_2
#define USARTx_TX_GPIO_PORT           GPIOA
#define USARTx_SET_TX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7)
#define USARTx_RX_PIN                 LL_GPIO_PIN_3
#define USARTx_RX_GPIO_PORT           GPIOA
#define USARTx_SET_RX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7)


void UartInit(void);
Bool UartCharAvail(void);
char UartCharRead(void);
void UartCharWrite(char OutChar);
void UartIrqHandler(void);

#endif /* UART_H_ */
