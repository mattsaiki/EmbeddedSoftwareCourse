/*
 * morse.c
 *
 *  Created on: Sep 26, 2017
 *      Author: Matthew Saiki
 */

#include "comp494.h"
#include "gpio.h"
#include "morse.h"
#include <ctype.h>

#define MORSE_DOT_ST_INIT		0
#define MORSE_DOT_ST_DLY		1
#define MORSE_DOT_FINISH		2

#define MORSE_DASH_ST_INIT		0
#define MORSE_DASH_ST_DLY		1
#define MORSE_DASH_FINISH		2

#define DLY_CNT_DOT				4
#define DLY_CNT_DASH			12
#define DLY_CNT_PAUSE			12
#define DLY_CNT_WORD			16
#define DLY_CNT_TICK			10000

#define MORSE_ST_INIT			0
#define MORSE_ST_DOT			1
#define MORSE_ST_DASH			2
#define MORSE_ST_CHECK_NEXT		3
#define MORSE_ST_WORD_PAUSE		4
#define MORSE_ST_IDLE			5

#define MORSE_PAUSE_ST_INIT		0
#define MORSE_PAUSE_ST_DLY		1

#define RTN_INCOMPLETE			0
#define RTN_COMPLETE 			1

const int Letters[26][6] =
{
		{1,2,-1,-1,-1,-1}, {2,1,1,1,-1,-1}, {2,1,2,1,-1,-1}, {2,1,1,-1,-1,-1}, {1,-1,-1,-1,-1,-1}, {1,1,2,1,-1,-1},
		{2,2,1,-1,-1,-1}, {1,1,1,1,-1,-1}, {1,1,-1,-1,-1,-1}, {1,2,2,2,-1,-1}, {2,1,2,-1,-1,-1}, {1,2,1,1,-1,-1},
		{2,2,-1,-1,-1,-1}, {2,1,-1,-1,-1,-1}, {2,2,2,-1,-1,-1}, {1,2,2,1,-1,-1}, {2,2,1,2,-1,-1}, {1,2,1,-1,-1,-1},
		{1,1,1,-1,-1,-1}, {2,-1,-1,-1,-1,-1}, {1,1,2,-1,-1,-1}, {1,1,1,2,-1,-1}, {1,2,2,-1,-1,-1,}, {2,1,1,2,-1,-1},
		{2,1,2,2,-1,-1}, {2,2,1,1,-1,-1}
};

const int Numbers[10][6] =
{
		{2,2,2,2,2,-1}, {1,2,2,2,2,-1}, {1,1,2,2,2,-1}, {1,1,1,2,2,-1}, {1,1,1,1,2,-1},
		{1,1,1,1,1,-1}, {2,1,1,1,1,-1}, {2,2,1,1,1,-1}, {2,2,2,1,1,-1}, {2,2,2,2,1,-1}
};

int IdleState;
static char* MorseMessage;
static int State;

static int NextState(int MessagePos, int MorsePos);
static int DotStateMachineClock(void);
static int DashStateMachineClock(void);
static int MorseWordPauseStateMachineClock(void);
static int MorseLetterPauseStateMachineClock(void);

//Initializes the state machine
void MorseStateMachineInit(char *Message)
{
	State = MORSE_ST_INIT;
	MorseMessage = Message;
	IdleState = 0;
}

//Morse State machine
void MorseStateMachineClock()
{
	static int MessagePosition;
	static int MorsePosition;

	switch(State)
	{
	case MORSE_ST_INIT:
		if(MorseWordPauseStateMachineClock() == RTN_COMPLETE)
		{
			MessagePosition = 0;
			MorsePosition = 0;
			State = NextState(MessagePosition, MorsePosition);
		}
		break;
	case MORSE_ST_DOT:
		if(DotStateMachineClock() == RTN_COMPLETE)
		{
			MorsePosition++;
			State = NextState(MessagePosition, MorsePosition);
		}
		break;
	case MORSE_ST_DASH:
		if(DashStateMachineClock() == RTN_COMPLETE)
		{
			MorsePosition++;
			State = NextState(MessagePosition, MorsePosition);
		}
		break;
	case MORSE_ST_CHECK_NEXT:
		if(MorseLetterPauseStateMachineClock() == RTN_COMPLETE)
		{
			MessagePosition++;
			MorsePosition = 0;
			State = NextState(MessagePosition, MorsePosition);
		}
		break;
	case MORSE_ST_WORD_PAUSE:
		if(MorseWordPauseStateMachineClock() == RTN_COMPLETE)
		{
			MessagePosition++;
			MorsePosition = 0;
			State = NextState(MessagePosition, MorsePosition);
		}
		break;
	case MORSE_ST_IDLE:
		IdleState = 1;
		break;
	}
}

//Returns 1 if machine is idle, 0 if it isn't
int MorseStateMachineIsIdle()
{
	return IdleState;
}

//Returns what the next state is
int NextState(int MessagePos, int MorsePos)
{
	char Symbol = MorseMessage[MessagePos];
	if(Symbol != '\0')
	{
		if(Symbol == ' ')
		{
			return MORSE_ST_WORD_PAUSE;
		}
		int RowNum;
		//Its a letter
		if (Symbol >= 'A' && Symbol <= 'Z')
		{
			RowNum = (Symbol - 'A');
			if(Letters[RowNum][MorsePos] == 1)
			{
				return MORSE_ST_DOT;
			}
			else if (Letters[RowNum][MorsePos] == 2)
			{
				return MORSE_ST_DASH;
			}
			//Reached end of symbol's morse
			else
			{
				return MORSE_ST_CHECK_NEXT;
			}
		}
		//Its a number
		else
		{
			int Num = (Symbol - '0');
			if(Numbers[Num][MorsePos] == 1)
			{
				return MORSE_ST_DOT;
			}
			else if (Numbers[Num][MorsePos] == 2)
			{
				return MORSE_ST_DASH;
			}
			else
			{
				return MORSE_ST_CHECK_NEXT;
			}
		}
	}
	else
	{
		return MORSE_ST_IDLE;
	}
}

//Turns on the light, pauses, turns off the light
int DotStateMachineClock(void)
{
	static int MorseDotState = MORSE_DOT_ST_INIT;
	static int DotDlyCnt;
	static int FinishDlyCnt;
	switch(MorseDotState)
	{
	case MORSE_DOT_ST_INIT:
		GPIO_LED_ENA();
		DotDlyCnt = DLY_CNT_DOT;
		FinishDlyCnt = DLY_CNT_DOT;
		MorseDotState = MORSE_DOT_ST_DLY;
		break;
	case MORSE_DOT_ST_DLY:
		if(--DotDlyCnt == 0)
		{
			MorseDotState = MORSE_DOT_FINISH;
			GPIO_LED_DIS();
		}
		break;
	case MORSE_DOT_FINISH:
		if(--FinishDlyCnt == 0)
		{
			MorseDotState = MORSE_DOT_ST_INIT;
			return RTN_COMPLETE;
		}
		break;
	}
	return RTN_INCOMPLETE;
}

//Turns on the light, pauses, turns off the light
int DashStateMachineClock(void)
{
	static int MorseDashState = MORSE_DASH_ST_INIT;
	static int DashDlyCnt;
	static int FinishDlyCnt;
	switch(MorseDashState)
	{
	case MORSE_DASH_ST_INIT:
		GPIO_LED_ENA();
		DashDlyCnt = DLY_CNT_DASH;
		FinishDlyCnt = DLY_CNT_DOT;
		MorseDashState = MORSE_DASH_ST_DLY;
		break;
	case MORSE_DASH_ST_DLY:
		if(--DashDlyCnt == 0)
		{
			MorseDashState = MORSE_DASH_FINISH;
			GPIO_LED_DIS();
		}
		break;
	case MORSE_DASH_FINISH:
		if(--FinishDlyCnt == 0)
		{
			MorseDashState = MORSE_DASH_ST_INIT;
			return RTN_COMPLETE;
		}
		break;
	}
	return RTN_INCOMPLETE;
}

//Pauses between words
int MorseWordPauseStateMachineClock(void)
{
	static int MorsePauseState = MORSE_PAUSE_ST_INIT;
	static int PauseDlyCnt;
	switch(MorsePauseState)
	{
	case MORSE_PAUSE_ST_INIT:
		PauseDlyCnt = DLY_CNT_WORD;
		MorsePauseState = MORSE_PAUSE_ST_DLY;
		break;
	case MORSE_PAUSE_ST_DLY:
		if(--PauseDlyCnt == 0)
		{
			MorsePauseState = MORSE_PAUSE_ST_INIT;
			return RTN_COMPLETE;
		}
		break;
	}
	return RTN_INCOMPLETE;
}

//Pauses between letters
int MorseLetterPauseStateMachineClock(void)
{
	static int MorsePauseState = MORSE_PAUSE_ST_INIT;
	static int PauseDlyCnt;
	switch(MorsePauseState)
	{
	case MORSE_PAUSE_ST_INIT:
		PauseDlyCnt = DLY_CNT_PAUSE;
		MorsePauseState = MORSE_PAUSE_ST_DLY;
		break;
	case MORSE_PAUSE_ST_DLY:
		if(--PauseDlyCnt == 0)
		{
			MorsePauseState = MORSE_PAUSE_ST_INIT;
			return RTN_COMPLETE;
		}
		break;
	}
	return RTN_INCOMPLETE;
}
