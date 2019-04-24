/***************************************************************************//**
 * @file    main.c
 * @author  <Vanshaj Taxali>
 * @email   <vanshajtaxali94@gmail.com>
 * @date    <05/05/2018>
 *
 * @brief   Exercise 1 - Display Interface
 *
 * Description and pin connections go here.
 *
 * @note    The project was exported using CCS 8.0.0.
 ******************************************************************************/
#include <msp430g2553.h>
#include "libs/LCD.h"
#include "libs/templateEMP.h"

// defining right and left shift
#define SHIFT_LEFT  0x18
#define SHIFT_RIGHT 0x1C


/****************************************Function Declaration************************/
void cursorfunc();
void charfunc();
void numfunc();
void shift(const char x);
/******************************************Main Function******************************/
int main(void)
{
    initMSP();
    lcd_init();                 // Initialize LCD
    lcd_clear();
    lcd_cursorSet(1, 0);        // Set the cursor position x/y
    lcd_putText("Welcome");
    lcd_cursorSet(2, 1);
    lcd_cursorShow(0);
    __delay_cycles(500000);// Set the cursor position x/y
    lcd_putText("MSP430g2553!");
    lcd_cursorShow(0);
    __delay_cycles(2000000);
    lcd_clear();


while(1)
{    lcd_clear();

     cursorfunc(); // display basic cursor functions like 'on', 'off', 'blink'.
     __delay_cycles(3000000);
     charfunc(); // implementing a charachter and string using putchar and put text
     __delay_cycles(4000000);
     numfunc();  // implementing a number using putnumber()
     lcd_cursorSet(4, 0);
     lcd_cursorShow(0);
     lcd_putText("Made By Vanshaj");
     shift('L');    // left shift
     lcd_clear();
     lcd_cursorSet(1, 0);
     lcd_cursorShow(0);
     lcd_putText("MTR Nr:4558621"); //right shift
     shift('R');
}
}
void cursorfunc()  //basic implementation of cursor functions
{
    lcd_cursorSet(1, 0);
       lcd_putText("Cursor Functions:");
       lcd_cursorSet(0, 1);
       lcd_putText("Off");
       lcd_cursorShow(0);
       __delay_cycles(2000000);
       lcd_clear();
       lcd_cursorSet(0, 1);
       lcd_putText("On");
       lcd_cursorShow(1);
       __delay_cycles(3000000);
       lcd_clear();
       lcd_cursorSet(0, 1);
       lcd_putText("Blink");
       lcd_cursorBlink(1);
       __delay_cycles(3000000);
       lcd_clear();
}
void charfunc()
{
    lcd_clear();
          lcd_cursorSet(1, 0);
          lcd_putText("Char Function:");
          lcd_cursorSet(0, 1);
          lcd_cursorBlink(1);
          lcd_putChar('V');
          __delay_cycles(3000000);
          lcd_clear();
          lcd_cursorSet(5, 0);
          lcd_putText("String:");
          lcd_cursorShow(0);
          __delay_cycles(2000000);
          lcd_cursorSet(0, 1);
          lcd_putText("Texas Instrument");
          __delay_cycles(3000000);
}
void numfunc()
{   lcd_clear();
    lcd_cursorSet(5, 0);
    lcd_putText("Number");
    lcd_cursorShow(0);
    __delay_cycles(500000);
    lcd_cursorSet(0, 1);
    unsigned int x;
    for (x=0; x<100; x++)
    {
                lcd_cursorSet(0, 1);
                __delay_cycles(20000);
                lcd_putNumber(x);           // show numbers
                lcd_cursorShow(0);
                __delay_cycles(50000);
                lcd_clear();
    }
    lcd_clear();
}
void shift(const char x ) // function for left shift and right shift
{
    int m;
    for (m=30; m>0; m--){
        if (x=='L')
            lcdcontrol(SHIFT_LEFT); //shift left defined above
        if (x=='R')
            lcdcontrol(SHIFT_RIGHT);//shift right defined above
        __delay_cycles(300000);
    }


}
