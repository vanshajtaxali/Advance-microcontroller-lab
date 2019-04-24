/***************************************************************************//**
 * @file    main.c
 * @author  <Vanshaj Taxali :Mtr Nr-4558621>
 * @date    <02/06/2018>
 *
 * @brief   Exercise 3 - Advanced Sensors
 *
 * Description and pin connections go here.
 *
 * @note    The project was exported using CCS 8.0.0.
 *
 ******************************************************************************/

#include "libs/templateEMP.h"   // UART disabled, see @note!
#include "libs/mma.h"
#include "libs/LCD.h"

#define MMA_INT1    BIT4                //MMA interrupt pin

void perform_selftest(void);
void pulse_interrupt(void);
void tap_detected(unsigned char pulse_src);

unsigned char rxArray[8] = {0};         // i2c read buffer
unsigned char axis_pulse, intFlag;
_Bool tap_source = 0;
unsigned int timerCount = 0;
unsigned int CountFlag = 0;



/********************************************************
 * MAIN - program starts here
 ********************************************************/
int main(void)
{

// Initializations
//*******************************************************
    // Initialization of the controller
    initMSP(),

    // Initialize LCD
    lcd_init();
    __delay_cycles(1000);           //Give time before another process


    // Initialize MMA8451 - ACCELEROMETER
    mma_init();

    __delay_cycles(1000);           //Give time before another init

    // set MMA8451 Resoultion to 14 Bit
    mma_setResolution(1);

    // Performs selftest on the MMA8451.
    perform_selftest();

    // Initialize/Enable pulse interrupt
    pulse_interrupt();



    while (1)
    {

    }

}

/********************************************************************
 * Function performs selftest on the MMA8451.
 * Repeats the process consecutively for 7-times, if test had failed
 *********************************************************************/
void perform_selftest()
{
    unsigned char selftest_count = 10;      // selftest count
    int  ctr, arr[10] = {0};

    for (ctr = 0; ctr < 10; ctr++)
    {
        arr[ctr] = mma_selftest();
        __delay_cycles(500000);
    }

    lcd_cursorShow(1);              // cursor on
    lcd_cursorSet(1, 0);            // Set the cursor position x/y
    lcd_putText("Performing... ");
    lcd_cursorSet(2, 1);            // Set the cursor position x/y
    lcd_putText("SelfTest...");

    __delay_cycles(3000000);
    lcd_clear();

    do{
        if(arr[selftest_count]){
            __delay_cycles(2000000);
            lcd_clear();

            lcd_cursorSet(0, 0);        // Set the cursor position (column, row)
            lcd_putText("Retry SelfTest");
            lcd_cursorSet(0, 1);        // Set the cursor position (column, row)
            lcd_cursorShow(1);          // cursor on
            lcd_putNumber(selftest_count);
            lcd_cursorSet(3, 1);        // Set the cursor position (column, row)
            lcd_putText(": Failed!");

            selftest_count--;
        }else{
            __delay_cycles(2000000);
            lcd_clear();
            lcd_cursorSet(0, 0);        // Set the cursor position (column, row)
            lcd_cursorShow(0);          // cursor on
            lcd_putText("SelfTest Passed");
            lcd_cursorSet(0, 1);        // Set the cursor position (column, row)
            lcd_cursorShow(0);          // cursor on
            lcd_putText("Sensor is Ready");

            break;
        }
    }while (selftest_count != 0);

    if(selftest_count == 0){
        __delay_cycles(2000000);
        lcd_clear();
        lcd_cursorSet(0, 0);            // Set the cursor position x/y
        lcd_putText("SelfTest Failed");
        lcd_cursorSet(0, 1);
        lcd_cursorShow(1);              // cursor on// Set the cursor position x/y
        lcd_putText("Permanently");

        __delay_cycles(2000000);
        lcd_clear();
        lcd_cursorSet(0, 0);            // Set the cursor position x/y
        lcd_putText("Can't Proceed");
        lcd_cursorSet(0, 1);
        lcd_cursorShow(0);              // cursor on// Set the cursor position x/y
        lcd_putText("Restart selftest");

        for (;;);       // Don't continue if self test failed permanently
    }
}

//********************************************************************
// function sets up double tap interrupt
//********************************************************************
void pulse_interrupt(void)
{
    __delay_cycles(2000000);
    lcd_clear();
    lcd_cursorSet(0, 0);    // Set the cursor position x/y
    lcd_putText("Setting up");
    lcd_cursorSet(0, 1);
    lcd_cursorShow(0);      // cursor on// Set the cursor position x/y
    lcd_putText("Double tap");

    __delay_cycles(2000000);
    lcd_clear();
    lcd_cursorShow(0);
    lcd_cursorSet(0, 0);    // Set the cursor position x/y
    lcd_putText("Going to sleep");
    lcd_cursorShow(0);      // cursor on// Set the cursor position x/y
    lcd_cursorSet(0, 1);
    lcd_putText("Tap me Later");


    // Enable MMA interrupt
    mma_enableTapInterrupt();

    P1DIR &= ~MMA_INT1;     // set MMA_INT1 pin as input
    P1REN |= MMA_INT1;      // enable pullup resistor
    P1IE  |= MMA_INT1;      // enable interrupt on MMA_INT1 pin
    P1IES |= MMA_INT1;      // trigger interrupt on lo/hi edge
    P1IFG &= ~MMA_INT1;     // clear interrupt flag

    __delay_cycles(1000000);

    intFlag = 1;

    // Enter LPM4 w/interrupt - deepest sleep level
    __bis_SR_register(LPM4_bits + GIE);
}


//********************************************************************
// function detects tap
//********************************************************************
void tap_detected(unsigned char pulse_src)
{
    timerCount = 0;         // reset counter

    lcd_clear();
    lcd_cursorSet(0, 0);    // Set the cursor position x/y
    lcd_putText("I Woke Up");
    lcd_cursorShow(1);
    __delay_cycles(2000000);

    //Go to sleep after 5 seconds ogf no activity
    do{
        mma_read();

        //Display the values
        lcd_clear();
        lcd_cursorSet(0, 0);    // Set the cursor position x/y
        lcd_putText("X:");
        lcd_cursorSet(2, 0);    // Set the cursor position x/y
        lcd_putNumber(mma_get14X());

       __delay_cycles(100000);
        lcd_cursorSet(0, 1);
        lcd_putText("Y:");
        lcd_cursorShow(0);      // cursor on// Set the cursor position x/y
        lcd_cursorSet(2, 1);    // Set the cursor position x/y
        lcd_putNumber(mma_get14Y());

        __delay_cycles(100000);
        lcd_cursorSet(8, 0);
        lcd_putText("Z:");
        lcd_cursorShow(0);      // cursor on// Set the cursor position x/y
        lcd_cursorSet(11, 0);   // Set the cursor position x/y
        lcd_putNumber(mma_get14Y());

        CountFlag = CountFlag + 1;
        __delay_cycles(1000000);

    }
    while(CountFlag < 5);

    // reset timer flag
    CountFlag = 0;

    lcd_clear();
    lcd_cursorSet(0, 0);
    lcd_cursorShow(0);
    lcd_putText("Sleeping");

    // Enter LPM4 w/interrupt - deepest sleep level
    __bis_SR_register(LPM0_bits + GIE);

}


//********************************************************************
//PORT1 interrupt service routine
//********************************************************************
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    CountFlag = 0;              // reset counter when tap is detected

    P1IFG &= ~MMA_INT1;     // clear interrupt flag
    tap_source = mma_disableTapInterrupt();

    if(intFlag)
        tap_detected(tap_source);
}
