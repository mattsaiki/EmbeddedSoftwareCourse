/*
 * eprintf.c
 *
 *  Created on: Oct 31, 2017
 *      Author: Matthew
 */
/*
 * COMP 494 - Matt Saiki
 *
 * eprintf() module
 */

#include <stdarg.h>
#include "comp494.h"

extern void __io_putchar(char CharToSend);
void PadWithZeros(int NumPads);
void PadWithSpace(int NumPads);

int eprintf(char * FmtStrPtr, ...)
{
	va_list 	ArgPtr;
	int			StrLen = 0;
	char *		StrPtr;
	int 		DecimalNumber;
	int			HexInput;

	//Arrays for decimal and hex conversion
	int NumberArray[12];
	int CharArray[12];
	//Variables for conversions
	int Remainder;
	int CurrentIndex;
	int NumDigits;
	int Quotient;
	int LeftConversion;
	int PadWithZero;
	int i;
	char ConvertedChar;

	va_start(ArgPtr, FmtStrPtr);

	while(*FmtStrPtr)
	{
		if(*FmtStrPtr != '%')
		{
			__io_putchar(*FmtStrPtr++);
			StrLen++;
			continue;
		}
		FmtStrPtr++;
		//If specifying left alignment
		if(*FmtStrPtr == '-')
		{
			LeftConversion = 1;
			FmtStrPtr++;
		}
		//Specifying right alignment
		else
		{
			LeftConversion = 0;
			//Zero padded, only for right justified elements
			if(*FmtStrPtr == '0')
			{
				PadWithZero = 1;
				FmtStrPtr++;
			}
			else
			{
				PadWithZero = 0;
			}
		}
		int CharToInt;
		int Multiplier = 10;
		int MinFieldWidth = 0;
		int MinNumSpaces = 0;
		//Read in the minimum field width if possible
		while(*FmtStrPtr >= '0' && *FmtStrPtr <= '9' )
		{
			CharToInt = *FmtStrPtr - '0';
			MinNumSpaces++;
			FmtStrPtr++;
			MinFieldWidth = (MinFieldWidth * Multiplier) + CharToInt;
		}

		switch(*FmtStrPtr)
		{
		case 'c':
			//Left align
			if(LeftConversion == 1)
			{
				//Send the character before the padding
				__io_putchar(va_arg(ArgPtr, int));
				StrLen++;
				//Pad with ' '
				if(MinFieldWidth != 0)
				{
					PadWithSpace(MinFieldWidth-1);
					StrLen += (MinFieldWidth-1);
				}
			}
			//Right align
			else
			{
				if(MinFieldWidth != 0)
				{
					if(PadWithZero == 1)
					{
						PadWithZeros(MinFieldWidth-1);
					}
					else
					{
						PadWithSpace(MinFieldWidth-1);
					}
					StrLen += (MinFieldWidth-1);
				}
				__io_putchar(va_arg(ArgPtr, int));
				StrLen++;
			}
			break;
		case 's':
			StrPtr = va_arg(ArgPtr, char *);
			//Left align
			if(LeftConversion == 1)
			{
				while(*StrPtr)
				{
					__io_putchar(*StrPtr++);
					StrLen++;
				}
				//Pad with ' '
				if(MinFieldWidth != 0)
				{
					PadWithSpace(MinFieldWidth-1);
					StrLen += (MinFieldWidth-1);
				}
			}
			//Right align
			else
			{
				if(MinFieldWidth != 0)
				{
					//Pad with ' '
					if(PadWithZero == 1)
					{
						PadWithZeros(MinFieldWidth-1);
					}
					//Pad with zeros
					else
					{
						PadWithSpace(MinFieldWidth-1);
					}
					StrLen += (MinFieldWidth-1);
				}
				while(*StrPtr)
				{
					__io_putchar(*StrPtr++);
					StrLen++;
				}
			}
			break;
		case 'd':
			DecimalNumber = va_arg(ArgPtr, int);
			int DecimalNumberCopy = DecimalNumber;
			NumDigits = 0;
			int DivideByTen;
			char PrintChar;
			//If number to be printed is 0
			if(DecimalNumber == 0)
			{
				if(LeftConversion == 1)
				{
					PrintChar = '0' + DecimalNumber;
					__io_putchar(PrintChar);
					StrLen++;
					if(MinFieldWidth != 0)
					{
						PadWithZeros(MinFieldWidth - 1);
						StrLen += (MinFieldWidth - 1);
					}
					break;
				}
				else
				{
					if(MinFieldWidth != 0)
					{
						if(PadWithZero == 1)
						{
							PadWithZeros(MinFieldWidth - 1);
						}
						else
						{
							PadWithSpace(MinFieldWidth - 1);
						}
						StrLen += (MinFieldWidth -1);
					}
					PrintChar = '0' + DecimalNumber;
					__io_putchar(PrintChar);
					StrLen++;
					break;
				}
			}
			//Figure out the number of decimal places
			while(DecimalNumberCopy != 0)
			{
				DecimalNumberCopy /= 10;
				NumDigits++;
			}
			CurrentIndex = NumDigits;
			//Put decimal places into array starting at the rightmost side
			while(DecimalNumber != 0)
			{
				DivideByTen = DecimalNumber/10;
				DivideByTen *= 10;
				Remainder = DecimalNumber - DivideByTen;
				NumberArray[CurrentIndex-1] = Remainder;
				CurrentIndex--;
				StrLen++;
				DecimalNumber /= 10;
			}
			CurrentIndex = NumDigits;
			if(LeftConversion == 1)
			{
				//Send characters in correct order
				for(int i = 0; i < NumDigits; i++)
				{
					__io_putchar('0'+NumberArray[i]);
					StrLen++;
				}
				if(MinFieldWidth != 0)
				{
					PadWithSpace(MinFieldWidth - 1);
					StrLen += (MinFieldWidth - 1);
				}
			}
			else
			{
				if(MinFieldWidth != 0)
				{
					if(PadWithZero == 1)
					{
						PadWithZeros(MinFieldWidth - 1);
					}
					else
					{
						PadWithSpace(MinFieldWidth - 1);
					}
					StrLen += (MinFieldWidth - 1);
					//Send characters in correct order
					for(int i = 0; i < NumDigits; i++)
					{
						__io_putchar('0'+NumberArray[i]);
						StrLen++;
					}
				}
			}
			break;
		case 'x':
			HexInput = va_arg(ArgPtr, int);
			NumDigits = 0;
			//If input is 0
			if(HexInput == 0)
			{
				if(LeftConversion == 1)
				{
					__io_putchar('0');
					StrLen++;
					if(MinFieldWidth != 0)
					{
						PadWithSpace(MinFieldWidth - 1);
						StrLen += (MinFieldWidth - 1);
					}
				}
				else
				{
					if(MinFieldWidth != 0)
					{
						if(PadWithZero == 1)
						{
							PadWithZeros(MinFieldWidth - 1);
						}
						else
						{
							PadWithSpace(MinFieldWidth - 1);
						}
						StrLen += (MinFieldWidth - 1);
					}
					__io_putchar('0');
					StrLen++;
				}
				break;
			}
			Quotient = HexInput;
			//Figure out the number of digits for hex representation
			while(Quotient != 0)
			{
				Quotient /= 16;
				NumDigits++;
				StrLen++;
			}
			Quotient = HexInput;
			CurrentIndex = NumDigits-1;
			//Fill in CharArray with corresponding Hex values
			while(Quotient != 0)
			{
				Remainder = Quotient % 16;
				Quotient /= 16;
				switch(Remainder)
				{
				case 10:
					ConvertedChar = 'a';
					break;
				case 11:
					ConvertedChar = 'b';
					break;
				case 12:
					ConvertedChar = 'c';
					break;
				case 13:
					ConvertedChar = 'd';
					break;
				case 14:
					ConvertedChar = 'e';
					break;
				case 15:
					ConvertedChar = 'f';
					break;
				default:
					ConvertedChar = '0' + Remainder;
				}
				CharArray[CurrentIndex] = ConvertedChar;
				CurrentIndex--;
			}
			if(LeftConversion == 1)
			{
				//Send characters in correct order
				for(int i = 0; i < NumDigits; i++)
				{
					__io_putchar(CharArray[i]);
					StrLen++;
				}
				if(MinFieldWidth != 0)
				{
					PadWithSpace(MinFieldWidth - 1);
					StrLen += (MinFieldWidth - 1);
				}
			}
			else
			{
				if(MinFieldWidth != 0)
				{
					if(PadWithZero == 1)
					{
						PadWithZeros(MinFieldWidth - 1);
					}
					else
					{
						PadWithSpace(MinFieldWidth - 1);
					}
					StrLen += (MinFieldWidth - 1);
				}
				//Send characters in correct order
				for(int i = 0; i < NumDigits; i++)
				{
					__io_putchar(CharArray[i]);
					StrLen++;
				}
			}
			break;
		case 'X':
			HexInput = va_arg(ArgPtr, int);
			NumDigits = 0;
			//If input is 0
			if(HexInput == 0)
			{
				if(LeftConversion == 1)
				{
					__io_putchar('0');
					StrLen++;
					if(MinFieldWidth != 0)
					{
						PadWithSpace(MinFieldWidth - 1);
						StrLen += (MinFieldWidth - 1);
					}
				}
				else
				{
					if(MinFieldWidth != 0)
					{
						if(PadWithZero == 1)
						{
							PadWithZeros(MinFieldWidth - 1);
						}
						else
						{
							PadWithSpace(MinFieldWidth - 1);
						}
						StrLen += (MinFieldWidth - 1);
					}
					__io_putchar('0');
					StrLen++;
				}
				break;
			}
			Quotient = HexInput;
			//Figure out the number of digits for hex representation
			while(Quotient != 0)
			{
				Quotient /= 16;
				NumDigits++;
				StrLen++;
			}
			Quotient = HexInput;
			CurrentIndex = NumDigits-1;
			//Fill in CharArray with corresponding Hex values
			while(Quotient != 0)
			{
				Remainder = Quotient % 16;
				Quotient /= 16;
				switch(Remainder)
				{
				case 10:
					ConvertedChar = 'A';
					break;
				case 11:
					ConvertedChar = 'B';
					break;
				case 12:
					ConvertedChar = 'C';
					break;
				case 13:
					ConvertedChar = 'D';
					break;
				case 14:
					ConvertedChar = 'E';
					break;
				case 15:
					ConvertedChar = 'F';
					break;
				default:
					ConvertedChar = '0' + Remainder;
				}
				CharArray[CurrentIndex] = ConvertedChar;
				CurrentIndex--;
			}
			if(LeftConversion == 1)
			{
				//Send characters in correct order
				for(int i = 0; i < NumDigits; i++)
				{
					__io_putchar(CharArray[i]);
					StrLen++;
				}
				if(MinFieldWidth != 0)
				{
					PadWithSpace(MinFieldWidth - 1);
					StrLen += (MinFieldWidth - 1);
				}
			}
			else
			{
				if(MinFieldWidth != 0)
				{
					if(PadWithZero == 1)
					{
						PadWithZeros(MinFieldWidth - 1);
					}
					else
					{
						PadWithSpace(MinFieldWidth - 1);
					}
					StrLen += (MinFieldWidth - 1);
				}
				//Send characters in correct order
				for(int i = 0; i < NumDigits; i++)
				{
					__io_putchar(CharArray[i]);
					StrLen++;
				}
			}
			break;
		case '%':
			if(LeftConversion == 1)
			{
				__io_putchar('%');
				StrLen++;
				if(MinFieldWidth != 0)
				{
					PadWithSpace(MinFieldWidth - 1);
					StrLen += (MinFieldWidth - 1);
				}
			}
			else
			{
				if(MinFieldWidth != 0)
				{
					if(PadWithZero == 1)
					{
						PadWithZeros(MinFieldWidth - 1);
					}
					else
					{
						PadWithSpace(MinFieldWidth - 1);
					}
					StrLen += (MinFieldWidth - 1);
				}
				__io_putchar('%');
				StrLen++;
			}

			StrLen++;
			break;
		default:
			__io_putchar('%');
			__io_putchar(*FmtStrPtr);
			StrLen++;
			break;
		}
		FmtStrPtr++;
	}
	va_end(ArgPtr);
	return StrLen;
}

//Function for padding with space
void PadWithSpace(int NumPads)
{
	for(int i = 0; i < NumPads; i++)
	{
		__io_putchar(' ');
	}
}

//Function for padding with zeros
void PadWithZeros(int NumPads)
{
	for(int i = 0; i < NumPads; i++)
	{
		__io_putchar('0');
	}
}
