/*
 * LCD.h
 *
 *  Created on: 07.05.2015
 *      Author: <fill in your name here>
 */

#include "./LCD.h"
//#include <templateEMP.h>
/*
 *
 * 	Here be dragons.
 *
 */

/** Initialisation */
// Initialisation of the LCD; set all pin directions, basic setup of the LCD, etc. (1 Pt.)
//The following is the initialization taken from the HD44780U datasheet, page 42
void lcd_init(void) {
	__delay_cycles(1000000); //50ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.
	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT |= BIT1;

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	__delay_cycles(100000);//5ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT |= BIT1;

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	//lcd_checkBusyFlag();

	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT |= (BIT3);	//N

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);	//F

	//DB5
	P2DIR |= BIT1;
	P2OUT &= (~BIT1);

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	//lcd_checkBusyFlag();

	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT &= (~BIT1);

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	//lcd_checkBusyFlag();

	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT |= BIT3;

	//DB6
	P2DIR |= BIT2;
	P2OUT |= BIT2;	//D

	//DB5
	P2DIR |= BIT1;
	P2OUT |= BIT1;	//C

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);	//B

	enableTrigger();
	//lcd_checkBusyFlag();

	//------------------------------clear dislpay----------------------
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT &= (~BIT1);

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	//lcd_checkBusyFlag();

	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT &= (~BIT1);

	//DB4
	P2DIR |= BIT0;
	P2OUT |= BIT0;

	enableTrigger();
	//lcd_checkBusyFlag();

	//-----------------------------Entry Mode------------------------

	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT &= (~BIT1);

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	//lcd_checkBusyFlag();

	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT |= BIT2;

	//DB5
	P2DIR |= BIT1;
	P2OUT |= BIT1;

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	//lcd_checkBusyFlag();

	__delay_cycles(50000);//50ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.
}

//the following sets the required outputs, and waits for the busy flag to turn of
void lcd_checkBusyFlag(void) {
	//Set RS and R/W directions to "output"
	//P3DIR |= (BIT0 | BIT1);	// set
	//RS
	//P3OUT &= (~BIT0);

	//R/W
	//P3OUT |= BIT1;

	//Set DB7 direction to "input"
	//P2DIR &= (~BIT3);//this sets the third bit by 0 while maintaining the rest of the bits
	//Read DB7
	//enableTrigger();

	//enableTrigger();
	while ((lcd_nowaitCheck() & 0x80) > 0)
		//this will make sure that only the 3rd bit is sensitive to the loop condition
		;
	//P2DIR |= BIT3;
}

//return 1 if busy, 0 if not
unsigned char lcd_nowaitCheck(void) {

	unsigned char c, readc;
	//Set RS and R/W directions to "output"
	P3DIR |= (BIT0 | BIT1);	// set
	P2DIR &= ~(BIT3 | BIT2 | BIT1 | BIT0);

	//RS
	P3OUT &= (~BIT0);

	//R/W
	P3OUT |= BIT1;
	//__delay_cycles(1000); // short propagation delay

	//Set DB7 direction to "input"

	//Read DB7
	enableTrigger();

	readc = (P2IN & (BIT3 | BIT2 | BIT1 | BIT0));
	c = ((readc << 4) & 0xF0);						// Move 4 bits to high nibble while zeroing low nibble
	enableTrigger();

	readc = (P2IN & (BIT3 | BIT2 | BIT1 | BIT0));

	c |= ( readc & 0x0F ); 							// Or in 4 more bits to low nibble

	P2DIR |= (BIT3 | BIT2 | BIT1 | BIT0);
	P3OUT &= (~BIT1);
	return c;

}
/** Control functions */
// Enable (1) or disable (0) the display (i.e. hide all text) (0.5 Pt)
void lcd_enable(unsigned char on) {
	lcd_checkBusyFlag();

	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT &= (~BIT1);

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	//lcd_checkBusyFlag();

	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT |= BIT3;

	//DB6
	P2DIR |= BIT2;
	if (on == 1) {
		P2OUT |= BIT2;		//D
	} else if (on == 0) {
		P2OUT &= (~BIT2);		//D
	}

	//DB5
	P2DIR |= BIT1;
	P2OUT |= BIT1;//C -- later get from global variables or where can you find the state??

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);//B -- later get from global variables or where can you find the state??

	enableTrigger();

}
// Set the cursor to a certain x/y-position (0.5 Pt)
/*
 * The y is either 0 or 1, 0 is for the first line, while 1 is for the second line.
 * Although that will not be shown, as the initialization sets the display to work with line only
 * Bits 0 to 5 are to set the X position, so the 8 bits instruction command is composed of the following 1YXXXXXX
 */void lcd_cursorSet(unsigned char x, unsigned char y) {

	lcd_checkBusyFlag();

	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT |= BIT3;

	//DB6
	P2DIR |= BIT2;
	if (y == 1) {
		P2OUT |= BIT2;		//D
	} else if (y == 0) {
		P2OUT &= (~BIT2);		//D
	}

	//DB5
	P2DIR |= BIT1;
	P2OUT |= (BIT5 & x);

	//DB4
	P2DIR |= BIT0;
	P2OUT |= (BIT4 & x);

	enableTrigger();
	//lcd_checkBusyFlag();

	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	P2OUT = 0;
	//DB7
	P2DIR |= BIT3;
	P2OUT |= (BIT3 & x);

	//DB6
	P2DIR |= BIT2;
	P2OUT |= (BIT2 & x);

	//DB5
	P2DIR |= BIT1;
	P2OUT |= (BIT1 & x);

	//DB4
	P2DIR |= BIT0;
	P2OUT |= (BIT0 & x);

	enableTrigger();

}
// Show (1) or hide (0) the cursor (0.5 Pt)
void lcd_cursorShow(unsigned char on) {
	lcd_checkBusyFlag();

	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT &= (~BIT1);

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	//lcd_checkBusyFlag();

	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT |= BIT3;

	//DB6
	P2DIR |= BIT2;	//Display must always be on when turning on or off the blink
	P2OUT |= BIT2;		//D

	/*
	 * The logic so far is to have the blink and the cursor exclusive from each other, i.e if B is 1, then C is 0 and vice versa.
	 * Naturally, this can also be handled by the global variables, as we already the state of B and C at all time.
	 * and whenever one needs to be changed, we make sure the other stays the same. But the implementation below makes more sense from a practical point of view.
	 */
	// 	//DB5 and DB4
	P2DIR |= (BIT1 | BIT0);
	if (on == 1) {
		P2OUT |= BIT1;		//C
		P2OUT &= (~BIT0);		//B

	} else if (on == 0) {
		P2OUT &= (~BIT1);		//C
		P2OUT |= BIT0;		//B
	}
	enableTrigger();

}
// Blink (1) or don't blink (0) the cursor (0.5 Pt)
void lcd_cursorBlink(unsigned char on) {
	lcd_checkBusyFlag();

	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT &= (~BIT1);

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	//lcd_checkBusyFlag();

	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT |= BIT3;

	//DB6
	P2DIR |= BIT2;	//Display must always be on when turning on or off the blink
	P2OUT |= BIT2;		//D

	/*
	 * The logic so far is to have the blink and the cursor exclusive from each other, i.e if B is 1, then C is 0 and vice versa.
	 * Naturally, this can also be handled by the global variables, as we already the state of B and C at all time.
	 * and whenever one needs to be changed, we make sure the other stays the same. But the implementation below makes more sense from a practical point of view.
	 */

	//DB5 and DB4
	P2DIR |= (BIT1 | BIT0);
	if (on == 1) {
		P2OUT &= (~BIT1);		//C
		P2OUT |= BIT0;		//B
	} else if (on == 0) {
		P2OUT |= BIT1;		//C
		P2OUT &= (~BIT0);		//B
	}
	enableTrigger();

}

/** Data manipulation */
// Delete everything on the LCD (1 Pt.)
void lcd_clear(void) {

	lcd_checkBusyFlag();		//only proceed when busy flag is 0

	//------------------------------clear dislpay----------------------
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT &= (~BIT1);

	//DB4
	P2DIR |= BIT0;
	P2OUT &= (~BIT0);

	enableTrigger();
	//lcd_checkBusyFlag();

	//RS
	P3DIR |= BIT0;
	P3OUT &= (~BIT0);

	//R/W
	P3DIR |= BIT1;
	P3OUT &= (~BIT1);

	//DB7
	P2DIR |= BIT3;
	P2OUT &= (~BIT3);

	//DB6
	P2DIR |= BIT2;
	P2OUT &= (~BIT2);

	//DB5
	P2DIR |= BIT1;
	P2OUT &= (~BIT1);

	//DB4
	P2DIR |= BIT0;
	P2OUT |= BIT0;

	enableTrigger();
}
// Put a single character on the display at the cursor's current position (1 Pt.)
void lcd_putChar(const char character) {
	lcd_checkBusyFlag();		//only proceed when busy flag is 0

	P3DIR |= (BIT0 | BIT1);
	//RS
	P3OUT |= BIT0;

	//R/W
	P3OUT &= (~BIT1);

	P2DIR |= (BIT3 | BIT2 | BIT1 | BIT0);

	P2OUT = 0;

	P2OUT |= (character >> 4);//shift the character to write the high nibble first

	//__delay_cycles(100);//100us delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	enableTrigger();
	//lcd_checkBusyFlag();//only proceed when busy flag is 0

	P3DIR |= (BIT0 | BIT1);

	//RS
	P3OUT |= BIT0;

	//R/W
	P3OUT &= (~BIT1);

	P2DIR |= (BIT3 | BIT2 | BIT1 | BIT0);

	//Data
	P2OUT = 0;
	P2OUT |= character;		//write low nibble

	//__delay_cycles(100);//100us delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	enableTrigger();
}
// Show a given string on the display. If the text is too long to display, don't show the rest (i.e. don't break into the next line).
// You may change this to 'char *' if 'const char *' does not work for you. (1 Pt.)
void lcd_putText(char * text) {
	//*text++ = '\0';
	int i = 0;
	while (text[i] != '\0') {
		lcd_putChar(text[i++]);
		//remove next line later, or not. Depends on how the demonstration looks like
		//__delay_cycles(1000);//1ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.
	}

}
// Show a given number at the cursor's current position. Note that this is a signed variable! (1 Pt.)
void lcd_putNumber(int number) {
	char *text = itoa(number);		//convert integer to string

	lcd_putText(text);		//use putText to print the string

}

void enableTrigger(void) {
	P3DIR |= BIT2;
	//EN ON
	P3OUT |= BIT2;

	__delay_cycles(100);//2us delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	//EN OFF
	P3OUT &= (~BIT2);

	__delay_cycles(100);//2us delay. And as it was already mentioned, there are some overheads, so, not quite exact.
}
//------------------------------------------------------------------------------------------------------------------------

/*
 * The following function converts an integer to string.
 * It first allocates an array for the string with the size of a 19 digit integer + a sign character, + the null termination
 * The function points at the last position first, and works its way till the first character, in that case it can be a - sign
 * or nothing, depending if it is a +ve or a -ve number.
 * The way it works its way to the beginning is by getting i%10, which will always return the unit digit, then divides i by 10,
 * to get the digit in the thenth position into the unit position, and so on till digit position is 0.
 */
//------------------------------------------------------------------------------------------------------------------------
/* Copyright (C) 1989, 1990, 1991, 1992 Free Software Foundation, Inc.
 Written by James Clark (jjc@jclark.com)

 This file is part of groff.

 groff is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free
 Software Foundation; either version 2, or (at your option) any later
 version.

 groff is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License along
 with groff; see the file COPYING.  If not, write to the Free Software
 Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#define INT_DIGITS 19		/* enough for 64 bit integer */

char *itoa(int i) {
	/* Room for INT_DIGITS digits, - and '\0' */
	static char buf[INT_DIGITS + 2];
	char *p = buf + INT_DIGITS + 1; /* points to terminating '\0' */
	if (i >= 0) {
		do {
			*--p = '0' + (i % 10);
			i /= 10;
		} while (i != 0);
		return p;
	} else { /* i < 0 */
		do {
			*--p = '0' - (i % 10);
			i /= 10;
		} while (i != 0);
		*--p = '-';
	}
	return p;
}
