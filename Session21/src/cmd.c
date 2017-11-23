/*
 * cmd.c
 *
 *  Created on: Nov 19, 2017
 *      Author: Matthew
 */

#include "comp494.h"
#include "cmd.h"
#include "led.h"
#include "uart.h"
#include "gpio.h"
#include "eprintf.h"
#include <ctype.h>

#define CMD_REC			0
#define CMD_TOK			1
#define CMD_EXE			2

#define CMD_CHARS_MAX	100

#define NULL		((void *)0)

char CmdBuf[CMD_CHARS_MAX + 1];
static int TokenIndex[8];
static int TotalNumChars = 0;
static int NumTokens;

static Bool CmdReceive(void);
static void CmdTokenize(void);
static void CmdExecute(void);
static void CmdDisplayHelp(void);
static int getMsec(int Index);
static void InvalidCommand(void);
static void ZeroToken(void);
static void OneToken(void);
static void TwoToken(void);

void CmdProcessSMClock()
{
	static int CommandState = CMD_REC;
	switch(CommandState)
	{
	case CMD_REC:
		if(CmdReceive())
		{
			CommandState = CMD_TOK;
		}
		break;
	case CMD_TOK:
		CmdTokenize();
		CommandState = CMD_EXE;
		break;
	case CMD_EXE:
		CmdExecute();
		CommandState = CMD_REC;
		//Reset variables
		TotalNumChars = 0;
		NumTokens = 0;
		break;
	}
}

//Returns true when the user hits enter
static Bool CmdReceive()
{
	int i;
	if(UartCharAvail())
	{
		char RecChar = UartCharRead();
		//Echo characters
		UartCharWrite(RecChar);

		//Backspace
		if(RecChar == '\b')
		{
			if(TotalNumChars != 0)
			{
				TotalNumChars--;
				UartCharWrite(' ');
				UartCharWrite('\b');
			}
			return FALSE;
		}
		else
		{
			//Too long of a command
			if(TotalNumChars > 100)
			{
				InvalidCommand();
			}

			CmdBuf[TotalNumChars] = RecChar;
			//If user pressed enter
			if(RecChar == '\r')
			{
				UartCharWrite('\n');
				//Convert all characters to upper case
				for(i=0; i<TotalNumChars; i++)
				{
					if(isalpha(CmdBuf[i]) != 0)
					{
						CmdBuf[i] = toupper(CmdBuf[i]);
					}
				}
				return TRUE;
			}
			else
			{
				TotalNumChars++;
				return FALSE;
			}
		}
	}
	return FALSE;
}

//Keeps track of the indices of ' ' in TokenIndex
static void CmdTokenize()
{
	int i;
	NumTokens = 0;
	//Keep track of where the tokens are
	for(i = 0; i < TotalNumChars; i++){
		if(CmdBuf[i] == ' ')
		{
			TokenIndex[NumTokens] = i;
			NumTokens++;
		}
	}
}

//Executes command
static void CmdExecute()
{
	//Applies to help and ?
	if(NumTokens == 0)
	{
		ZeroToken();
	}
	//Applies to blink ena, blink dis, blink ?
	else if(NumTokens == 1)
	{
		OneToken();
	}
	//Applies to blink on <msec_on>, blink on ?, blink off <msec_off>, blink off ?
	else if(NumTokens == 2)
	{
		TwoToken();
	}
	//Invalid command
	else
	{
		InvalidCommand();
	}
}

static void CmdDisplayHelp()
{
	eprintf(" This is the list of valid commands\r\n help --> prints out commands with explanation \r\n ? --> prints out commands with explanation \r\n blink ena --> enables blinking LED \r\n"
			" blink dis --> disables blinking LED \r\n blink ? --> prints status of blink (ena or dis) \r\n blink on <msec_on> --> sets the duration for how long the LED should stay on \r\n"
			" blink on ? --> prints out the duration of the LED's on period \r\n blink off <msec_off> --> sets the duration for how long the LED should stay off \r\n blink off ? --> prints out the duration of the LED's off period \r\n");
}

static int GetMsec(int Index)
{
	int RetValue;
	int DigitValue;
	RetValue = 0;
	while(CmdBuf[Index] != '\r')
	{
		if(isdigit(CmdBuf[Index]) != 0)
		{
			RetValue *= 10;
			DigitValue = CmdBuf[Index] - '0';
			RetValue += DigitValue;
			Index++;
		}
		//Not a valid command
		else
		{
			return -1;
		}
	}
	return RetValue;
}

static void InvalidCommand()
{
	eprintf("Invalid Command please try again");
	eprintf("\r");
	eprintf("\n");
	TotalNumChars = 0;
	NumTokens = 0;
}

static void ZeroToken()
{
	if(CmdBuf[0] == '?')
	{
		CmdDisplayHelp();
	}
	else if(CmdBuf[0] == 'H' && CmdBuf[1] == 'E' && CmdBuf[2] == 'L' && CmdBuf[3] == 'P' && CmdBuf[4] == '\r')
	{
		CmdDisplayHelp();
	}
	else
	{
		InvalidCommand();
	}
}

static void OneToken()
{
	if(CmdBuf[0] == 'B' && CmdBuf[1] == 'L' && CmdBuf[2] == 'I' && CmdBuf[3] == 'N' && CmdBuf[4] == 'K' && CmdBuf[5] == ' ')
	{
		if(CmdBuf[6] == 'E' && CmdBuf[7] == 'N' && CmdBuf[8] == 'A' && CmdBuf[9] == '\r')
		{
			SetState(1);
		}
		else if(CmdBuf[6] == 'D' && CmdBuf[7] == 'I' && CmdBuf[8] == 'S' && CmdBuf[9] == '\r')
		{
			GPIO_LED_DIS();
			SetState(2);
		}
		else if(CmdBuf[6] == '?' && CmdBuf[7] == '\r')
		{
			int State = GetState();
			//Enabled
			if(State == 0 || State == 1)
			{
				eprintf("%s", "Enabled");
				eprintf("\r");
				eprintf("\n");
			}
			else
			{
				eprintf("%s", "Disabled");
				eprintf("\r");
				eprintf("\n");
			}
		}
		else
		{
			InvalidCommand();
		}
	}
	else
	{
		InvalidCommand();
	}
}

static void TwoToken()
{
	int SetVal;
	int GetVal;
	if(CmdBuf[0] == 'B' && CmdBuf[1] == 'L' && CmdBuf[2] == 'I' && CmdBuf[3] == 'N' && CmdBuf[4] == 'K' && CmdBuf[5] == ' ')
	{
		if(CmdBuf[6] == 'O' && CmdBuf[7] == 'N' && CmdBuf[8] == ' ')
		{
			if(CmdBuf[9] == '?' && CmdBuf[10] == '\r')
			{
				GetVal = GetOnPeriod();
				eprintf("%d", GetVal);
				eprintf("\r");
				eprintf("\n");
			}
			else
			{
				SetVal = GetMsec(9);
				if(SetVal == -1)
				{
					InvalidCommand();
				}
				else
				{
					SetOnPeriod(SetVal);
				}
			}
		}
		else if(CmdBuf[6] == 'O' && CmdBuf[7] == 'F' && CmdBuf[8] == 'F' && CmdBuf[9] == ' ')
		{
			if(CmdBuf[10] == '?' && CmdBuf[11] == '\r')
			{
				GetVal = GetOffPeriod();
				eprintf("%d", GetVal);
				eprintf("\r");
				eprintf("\n");
			}
			else
			{
				SetVal = GetMsec(10);
				if(SetVal == -1)
				{
					InvalidCommand();
				}
				else
				{
					SetOffPeriod(SetVal);
				}
			}
		}
		else
		{
			InvalidCommand();
		}
	}
	else
	{
		InvalidCommand();
	}
}
