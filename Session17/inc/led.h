/*
 * led.h
 *
 *  Created on: Oct 3, 2017
 *      Author: Matthew
 */

#ifndef LED_H_
#define LED_H_

void LedBlinkSMClock(void);
void LedBlinkSMInitOffMsec(int OffMsec);
void LedBlinkSMInitOnMsec(int OnMsec);

#endif /* LED_H_ */
