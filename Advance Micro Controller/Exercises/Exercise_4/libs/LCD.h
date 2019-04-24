/*
 * LCD.h
 *
 *  Created on: 07.05.2015
 *      Author: Sebastian Sester (sebastian.sester@imtek.de)
 */
#include <msp430g2553.h>
#include <stdint.h>
#ifndef LIBS_LCD_H_
#define LIBS_LCD_H_

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

void lcd_checkBusyFlag(void);
unsigned char lcd_nowaitCheck(void);
void enableTrigger(void);
char *itoa(int i);

#endif /* LIBS_LCD_H_ */
