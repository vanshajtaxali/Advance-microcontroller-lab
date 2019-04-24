/***************************************************************************//**
 * @file    main.c
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <16-06-2018>
 *
 * @brief   Exercise 4 - SPI
 *
 * Description and pin connections go here.
 *
 * @note    The project was exported using CCS 8.0.0.
 *
 ******************************************************************************/
#include <msp430g2553.h>
#include "libs/templateEMP.h"   // UART disabled, see @note!
#include "libs/lcd.h"
#include "libs/flash.h"
#include "libs/adac.h"
#include <stdint.h>
#include <stdbool.h>

unsigned char readADAC[5] = { 0 };    // stored data from joystick
int i=0;
void joystick_read();
void lcd_begin();
unsigned char s1[ ] = {"WELCOME"};
unsigned char s2[ ];
int main(void)
{
    initMSP();
while(1)
{
    lcd_begin();


    joystick_read();
    // at this perticular point I wanted to write it into the flash memory s1[];
    //then set the Isbusy flag in while loop such that if empty it exits else it keep
    //on reading from the flash
if(i==1)
{
      lcd_clear();
      lcd_cursorSet(0, 0);
      lcd_putText("1. WELCOME");
      lcd_cursorShow(0);
      i=0;

}
else if(i==2)
{
        lcd_clear();
      lcd_cursorSet(0, 0);
      lcd_putText("2. IM MSP430");
      lcd_cursorShow(0);
      i=0;
}


}
}
void lcd_begin()
{
    // Initialize LCD
    lcd_init();
    lcd_clear();
    __delay_cycles(100000);           //Give time before another process

    lcd_cursorSet(0, 0);            // Set the cursor position x/y
    lcd_putText("Select message");
    lcd_cursorShow(0);
    __delay_cycles(2000000);
    lcd_clear();
    lcd_cursorSet(0, 0);
    lcd_putText("1. WELCOME(UP)");
    lcd_cursorSet(0, 1);
    lcd_putText("2.IM MSP430 (DWN)");
    lcd_cursorShow(0);

}
void joystick_read()
{
    while(i==0)
    {
        // Get joystick positions
        adac_init();
      adac_read(readADAC);
// reading up and down
        if (readADAC[2] == 0x00)
                {
                    i=1;
                }
        if (readADAC[2] == 0xff)
                        {
                            i=2;
                        }
    }

}

/* Sorry could not correctly implement it as I am still confused with the interrupt handling and flash but the basic Idea was to display 2 kinds of messages in the beginning
 * and ask user to push joystick up or down for selecting the message and then writing that into the flash, later before
 * lcd_begin() inserting a while loop with condition for checking the flash memory (library modif) if not free read the previously stored
 * message every time else ask for user input.
 */

