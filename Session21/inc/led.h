//
// COMP 494 - Tom Lupfer
//
// LED module header file
//

#ifndef LED_H_
#define LED_H_

void LedBlinkSMClock(void);
int GetState(void);
void SetState(int State);
int GetOnPeriod(void);
void SetOnPeriod(int Period);
int GetOffPeriod(void);
void SetOffPeriod(int Period);

#endif /* LED_H_ */
