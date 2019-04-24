/***************************************************************************//**
 * @file    LCD.c
 * @author  <VANSHAJ TAXALI>
 * @Mtr Nr : 4558621
 * @date    <05.05.2017>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/
#ifndef LIBS_LCD_H_
#define LIBS_LCD_H_

#include <msp430g2553.h>
#include <string.h>
#include <stdio.h>

// Delay Functions
#define delay_ms(x)		__delay_cycles((long) x* 16000)
#define delay_us(x)		__delay_cycles((long) x* 16)


// Pins
#define EN		BIT2
#define RW		BIT1
#define RS		BIT0
#define lcdDATA	0x0F

// LCD commands
#define SHIFT_LEFT  0x18
#define SHIFT_RIGHT	0x1C
#define _4BIT_MODE  0x28
#define ENTRY_MODE	0x06
#define DISPLAY_OFF	0x08
#define NO_CURSOR	0x0C
#define STEADY_CURSOR	0x0E
#define BLINK_CURSOR	0x0F
#define CLEAR_DISPLAY	0x01
#define FIRST_LINE		0x80
#define SECOND_LINE		0x40
#define FIRST_LINE_REVERSE	0x60
#define SECOND_LINE_REVERSE	0x20


/** Initialisation */
// Initialisation of the LCD; set all pin directions, basic setup of the LCD, etc. (1 Pt.)
void lcd_init (void);

/** Control functions */
// Enable (1) or disable (0) the display (i.e. hide all text) (0.5 Pt)
void lcd_enable (unsigned char on);

// Set the cursor to a certain x/y-position (0.5 Pt)
void lcd_cursorSet (unsigned char x, unsigned char y);

// Show (1) or hide (0) the cursor (0.5 Pt)
void lcd_cursorShow (unsigned char on);

// Blink (1) or don't blink (0) the cursor (0.5 Pt)
void lcd_cursorBlink (unsigned char on);

/** Data manipulation */
// Delete everything on the LCD (1 Pt.)
void lcd_clear (void);

// Put a single character on the display at the cursor's current position (1 Pt.)
void lcd_putChar (const char character);

// Show a given string on the display. If the text is too long to display, don't show the rest (i.e. don't break into the next line).
// You may change this to 'char *' if 'const char *' does not work for you. (1 Pt.)
void lcd_putText (char * text);

// Show a given number at the cursor's current position. Note that this is a signed variable! (1 Pt.)
void lcd_putNumber (int number);

// Send LCD commands
void lcdCommand(unsigned char cmd);

#endif /* LIBS_LCD_H_ */
