/*
 * morse.h
 *
 *  Created on: Sep 26, 2017
 *      Author: Matthew
 */

#ifndef MORSE_H_
#define MORSE_H_


void MorseStateMachineInit(char *Message);
void MorseStateMachineClock();
int MorseStateMachineIsIdle();


#endif /* MORSE_H_ */
