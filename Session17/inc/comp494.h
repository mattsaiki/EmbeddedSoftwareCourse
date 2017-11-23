/*
 * comp494.h
 *
 *  Created on: Sep 26, 2017
 *      Author: Matthew
 */

#ifndef COMP494_H_
#define COMP494_H_

#include "stm32l1xx.h"
#include "stm32l1xx_nucleo.h"
#include "stm32l1xx_ll_usart.h"

#define USARTx_INSTANCE               USART2
#define USARTx_CLK_ENABLE()           LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define USARTx_GPIO_CLK_ENABLE()      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)   /* Enable the peripheral clock of GPIOA */
#define USARTx_TX_PIN                 LL_GPIO_PIN_2
#define USARTx_TX_GPIO_PORT           GPIOA
#define USARTx_SET_TX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7)
#define USARTx_RX_PIN                 LL_GPIO_PIN_3
#define USARTx_RX_GPIO_PORT           GPIOA
#define USARTx_SET_RX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7)

#define FALSE		0
#define TRUE		1

#define NULL_PTR		((void *) 0)

typedef int			Bool;

#endif /* COMP494_H_ */
