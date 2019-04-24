/*
 * Exercise 1
 *
 *  Created on: 07.05.2015
 *      Author: <fill in your name here>
 */

#include <templateEMP.h>
#include "libs/LCD.h"
//#include <templateEMP.h>

int main(void) {
    initMSP();
    // TODO (sesters): Add your initialization here.

    lcd_init();

    __delay_cycles(500000);//500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_cursorBlink(1);//blink cursor

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_cursorBlink(0);//stop blinking cursor

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_putChar('H');
    lcd_putChar('A');
    lcd_putChar('S');
    lcd_putChar('S');
    lcd_putChar('A');
    lcd_putChar('N');

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_enable(0);//turn off display, while not deleting the contents

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_enable(1);//turn on display, showing old contents

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.


    lcd_cursorShow(0);
    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_cursorShow(1);
    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_cursorSet(12, 0);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_cursorSet(12, 1);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.


    lcd_cursorSet(15, 0);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_cursorSet(22, 0);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_cursorSet(0, 1);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.


    lcd_cursorSet(0, 0);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_clear();

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.


    lcd_putText("Hello ");

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_putText("World! ");

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_clear();

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_putText("<16 characters.");

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_clear();

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_putText("More than 16 characters.");

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_clear();

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_putText("This sentence is more than fourty characters.");

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_clear();

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.


    lcd_putNumber(1234);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_clear();

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.


    lcd_putNumber(0000);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_clear();

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.


    lcd_putNumber(-1234);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.


    lcd_clear();

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.


    lcd_putNumber(12.3456);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

    lcd_clear();

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.


    lcd_putNumber(1234);

    __delay_cycles(2000000);//2000ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	lcd_clear();
    while (1) {
    	// TODO (sesters): Show a funky demo of what you did.
    }
}
