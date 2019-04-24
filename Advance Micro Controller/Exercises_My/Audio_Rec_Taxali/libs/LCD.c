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
#include "./LCD.h"

/** Initialisation */
// Initialisation of the LCD; set all pin directions, basic setup of the LCD, etc. (1 Pt.)
void lcd_init (void)
{
	delay_ms(100);
	// Wait for 100ms after power is applied.

	P3DIR |= EN + RW + RS;  // Make control lines output
	P3OUT &= ~RW;			// Set to low for constant write - P3.1 low
	P2DIR |= lcdDATA;  		// Make pins outputs

	/*start lcd, wait, set LCD to 4-Bit mode*/
	int i;
	for(i=3; i>0; i--)
	{
		P2OUT = 0x03;  		// Start LCD (send 0x03)
		lcd_enable(1);		// enable lcd
		lcd_enable(0);		// disable lcd
		delay_ms(100);		// wait
	}
	P2OUT = 0x02; 		// Switch to 4-bit mode
	lcd_enable(1);		// enable lcd
	lcd_enable(0);		// disable lcd
	delay_ms(10);		// wait

	lcdCommand(_4BIT_MODE); // Function Set: 4-bit, 2 Line, 5x7 Dots
	lcd_clear();			// Clear LCD
	lcdCommand(ENTRY_MODE);	// Entry Mode Set - cursor move direction, increment
	lcdCommand(NO_CURSOR); 	// Display On, No blink
	lcd_cursorSet(0, 0);	// Set the cursor to a certain x/y-position
}

/** Control functions */
// Enable (1) or disable (0) the display (i.e. hide all text) (0.5 Pt)
void lcd_enable (unsigned char on)
{
	if(on==1) P3OUT |= EN;	//P3.2 high
	else P3OUT &= ~EN;		//P3.2 low
}

// Set the cursor to a certain x/y-position (0.5 Pt)
void lcd_cursorSet (unsigned char x, unsigned char y)
{
	if (x < 16) {
		x |= FIRST_LINE; 			// Set LCD for first line write
		switch (y){
		case 1:
			x |= SECOND_LINE; 		 // Set LCD for second line write
			break;
		case 2:
			x |= FIRST_LINE_REVERSE; // Set LCD for first line write reverse
			break;
		case 3:
			x |= SECOND_LINE_REVERSE; // Set LCD for second line write reverse
			break;
		}
		lcdCommand(x);
	}
}

// Show (1) or hide (0) the cursor (0.5 Pt)
void lcd_cursorShow (unsigned char on)
{
	if (on == 0)
		lcdCommand(NO_CURSOR);		//display on, cursor off,
	else if (on == 1)
		lcdCommand(STEADY_CURSOR);	//display on, cursor on, steady cursor
}

// Blink (1) or don't blink (0) the cursor (0.5 Pt)
void lcd_cursorBlink (unsigned char on)
{
	if (on == 0)
		lcdCommand(STEADY_CURSOR);	//display on, cursor on, steady cursor
	else if (on == 1)
		lcdCommand(BLINK_CURSOR);	//display on, cursor on, blinking cursor
}

/** Data manipulation */
// Delete everything on the LCD (1 Pt.)
void lcd_clear (void)
{
	lcdCommand(DISPLAY_OFF); 			// Display off Cursor off (retain DDRAM content)
	lcdCommand(CLEAR_DISPLAY); 			// Clear Display (also clear DDRAM content)
}

// Put a single character on the display at the cursor's current position (1 Pt.)
void lcd_putChar (const char character)
{
	P3OUT |= RS; 	// Set RS to Data - P3.0 high
	P2OUT = (P2OUT & 0xF0) + ((character >> 4) & 0x0F);	// Upper nibble
	lcd_enable(1);
	lcd_enable(0);
	P2OUT = (P2OUT & 0xF0) + (character & 0x0F);		// Lower nibble
	lcd_enable(1);
	lcd_enable(0);
	delay_us(50); 	// Delay 50 us
}

// Show a given string on the display. If the text is too long to display, don't show the rest (i.e. don't break into the next line).
// You may change this to 'char *' if 'const char *' does not work for you. (1 Pt.)
void lcd_putText (char * text)
{
	unsigned int i = 0;
	while ((text[i] != '\0') && (i < 16)){
		lcd_putChar(text[i]);
		i++;
	}
}

// Show a given number at the cursor's current position. Note that this is a signed variable! (1 Pt.)
void lcd_putNumber (int number)
{
	char number_string[16];
	sprintf(number_string, "%d", number); // Convert the integer to character string
	lcd_putText(number_string);
}

// Send LCD commands
void lcdCommand(unsigned char cmd)
{
	P3OUT &= ~RS; 	// Set RS to Data - P3.2 low
	P2OUT = (P2OUT & 0xF0) + ((cmd >> 4) & 0x0F);	// Upper nibble
	lcd_enable(1);
	lcd_enable(0);
	P2OUT = (P2OUT & 0xF0) + (cmd & 0x0F);			// Lower nibble
	lcd_enable(1);
	lcd_enable(0);
	delay_ms(5); 	// Delay 5ms
}

