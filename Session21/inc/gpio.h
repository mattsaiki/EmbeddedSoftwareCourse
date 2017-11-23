//
// COMP 494 - Tom Lupfer
//
// GPIO module header file
//

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_gpio.h"

#define GPIO_DBG_DIS()			(LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4))
#define GPIO_DBG_ENA()			(LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4))

#define GPIO_LED_DIS()			(LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5))
#define GPIO_LED_ENA()			(LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5))
#define GPIO_LED_TOG()			(LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5))

#define GPIO_PB_IS_PRESSED()	(!LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13))


void GpioInit(void);

#endif /* GPIO_H_ */
