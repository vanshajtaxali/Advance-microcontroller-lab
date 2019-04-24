/***************************************************************************//**
 * @file    main.c
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <28-06-2018>
 *
 * @brief   Exercise 5 - Audio Recorder
 *
 * Description and pin connections go here.
 * All the usual connections from the previous exercises
 * additional include
 * CON 8
 * PWM to P3.6
 * MIC to P1.0
 *
 * @note    The project was exported using CCS 8.0.0.
 *
 ******************************************************************************/

#include "stdint.h"
#include "stdbool.h"
#include <msp430g2553.h>
#include "libs/flash.h"
#include "libs/audio.h"
#include "libs/adac.h"
#include "libs/LCD.h"

unsigned char sampleData[255];		// data from microphone or aux source
unsigned char sectors_Written = 0;// keeps track of amount of data save on flash
unsigned char *PlaySampleData;		// pointer to data to be played back

unsigned char readADAC[5] = { 0 };	// stored data from joystick

unsigned int timerCount_ms = 0;		// milliseconds counter
unsigned int timerCount_sec = 0;	// seconds counter
unsigned char recordTimeElapsed = 0;
unsigned char sampleRateFlag, sampleRateCount = 0;//20KHZ sample rate variables
unsigned char recordPlay, start_record_play;
unsigned char s1[] = { "WELCOME" };

/* configure_MSP */
/**********************/
unsigned char configure_MSP()
{
    // Stop watchdog timer
    WDTCTL = WDTPW + WDTHOLD;

    // initialize pins
    P1DIR = 0xFF;
    P1OUT = 0x00;
    P2DIR = 0xFF;
    P2OUT = 0x00;
    P3DIR = 0xFF;
    P3OUT = 0x00;

    // If calibration constant erased
    if ((CALBC1_16MHZ == 0xFF) || (CALBC1_16MHZ == 0xFF))
    {
        return 1;                        // do not load, return
    }
    DCOCTL = 0;                        // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_16MHZ;            // Set range to 16Mhz
    DCOCTL = CALDCO_16MHZ;			   // Set DCO step + modulation

    return 0;
}

/* Configure Timer and PWM */
/**********************/
void configure_Timer_PWM_Init(void)
{
    P3SEL |= PWM;						// use pin for special function
    P3DIR |= PWM;						// pin direction to output

    TA0CCR0 = Resolution;				// PWM period
    TA0CCR2 = 0;						// Timer A Capture/Compare 2
    TA0CCTL0 = CCIE;					// CCR0 interrupt enable
    TA0CCTL2 = OUTMOD_3;				// TA0CCTL2 - Set/Reset mode
    TA0CTL = TASSEL_2 + MC_3 + TAIE; 	// SMCLK, Timer in Up/Down mode
}

/* initialize ADC */
/**********************/
void ADC_init(void)
{
    // 16 clock ticks, ADC On
    ADC10CTL0 = ADC10SHT_2 + ADC10ON;
    // A0 input
    ADC10CTL1 = INCH_0;
    // Enable ADC input on P1.0
    ADC10AE0 = MIC;
}

/* Display usage information */
/**********************/
void lcd_main_menu(unsigned int oper)
{
    // Initialize LCD
    lcd_init();
    lcd_clear();
    __delay_cycles(1000);			//Give time before another process

    // Live streaming display
    if (oper == 1)
    {
        lcd_cursorShow(0);				// cursor on
        lcd_cursorSet(0, 0);			// Set the cursor position x/y
        lcd_putText("Go Live !!!");
        lcd_cursorSet(0, 1);			// Set the cursor position x/y
        lcd_putText("Record/playback");
    }
    else	// Record/Playback display
    {
        lcd_cursorShow(0);				// cursor on
        lcd_cursorSet(0, 0);			// Set the cursor position x/y
        lcd_putText("Up  :Record");
        lcd_cursorSet(0, 1);			// Set the cursor position x/y
        lcd_putText("Down:Play");
    }

}

/* Record data from microphone */
/**********************/
void record_data(void)
{
    //*********Convert analog microphone to digital***********
    //********************************************************
    unsigned char i = 0;
    unsigned char Resolution_8_bit;

//	/* set amplifier gain */
//	/**********************/
    isl_init();					// initialize the digital potentiometer
    __delay_cycles(1200);		// wait
    isl_set(0x30);				// set wiper value on th isl
    __delay_cycles(1000);		// wait

    do	// Get 255 samples at 20KHZ sampling rate
    {
        //conversion not done
        sampleRateFlag = 0;
        // Enable and start a new conversion
        ADC10CTL0 |= ENC + ADC10SC;
        // wait for sampling rate to end
        while (sampleRateFlag == 0)
            ;
        // From 10 to 8 bit resolution
        Resolution_8_bit = ADC10MEM >> 2;
        // Store measured value in buffer
        sampleData[i] = Resolution_8_bit;
        i++;							// next data

        //continue capturing until sector is full
    }
    while (i != 255);

    // PlaySampleData is now a pointer to sampleData;
    PlaySampleData = sampleData;
}

//*********Store captured data to flash memory************
//********************************************************
void save_data_to_flash(unsigned char page)
{
    if (page > 31)
        return;

    long int address = (M25P16_SECTOR_BYTE_SIZE * page);

    // initialize spi_flash
    flash_init();
    // write to flash
    flash_write(address, 255, sampleData);
}

//************Retreive data to flash memory***************
//********************************************************
void retreive_data_from_flash(unsigned char page)
{
    if (page > 31)
        return;

    long int address = (M25P16_SECTOR_BYTE_SIZE * page);

    // initialize spi_flash
    flash_init();
    // read from flash
    flash_read(address, 255, sampleData);
    // PlaySampleData is now a pointer to sampleData;
    PlaySampleData = sampleData;
}

//*********Store captured data to flash memory************
//********************************************************
void verify_recording_playback(void)
{
    // Record microphone data and save in buffer
    record_data();

    // wait
  // __delay_cycles(16000);

    // PlaySampleData is a pointer to sampleData;
    PlaySampleData = sampleData;

    // Keep playing tones stored in buffer
    audio_playBuffer();
}

//************************************
//  MAIN
//************************************
int main(void)
{
    unsigned char sector;
    unsigned char goLive = 0;

    //*********INITIALIZATIONS************
    //************************************

    // Set up the watchdog, pins and the speed here.
    configure_MSP();

//	 Initialize ADC
    ADC_init();

    // Set up Timer, PWM playback
    configure_Timer_PWM_Init();

    // setup LCD main menu display
    lcd_main_menu(0);

    while (1)
    {
        //************************************************
        /* this verifies the recording and playback part */
        // This is now part of the main program
        //************************************************
        // verify_recording_playback();
        //************************************************
        /* Joystick Usage options - Up   = Recording,
         * 						  - Down = Playing
         * 						  - Left = Go live
         * 						  - Right= Main menu
         * 						  - Center=ACK			*/
        //************************************************
        // initialize ADAC
        adac_init();
        // Get joystick positions
        adac_read(readADAC);
        // wait a bit
        //__delay_cycles(16000);

        // joystick is moved up --> Select recording
        //************************************************
        if (readADAC[3] == 0x00)
        {
            // go back to LCD main menu display
            lcd_main_menu(0);

            lcd_cursorShow(1);				// cursor on
            lcd_cursorBlink(0);				// blinking cursor off
            lcd_cursorSet(12, 0);			// Set the cursor position

            recordPlay = 1;					// set recording flag
            start_record_play = 0;			// don't start recording
        }

        // joystick is moved dowm --> Select play
        if (readADAC[3] == 0xFF)
        {
            // go back to LCD main menu display
            lcd_main_menu(0);

            lcd_cursorShow(1);				// cursor on
            lcd_cursorBlink(0);				// blinking cursor off
            lcd_cursorSet(12, 1);			// Set the cursor position

            recordPlay = 2;					// playing flag set
            start_record_play = 0;			// don't start playing
        }

        // Joystick pressed. Record/Play/Go Live acknowledge
        //************************************************
        if (readADAC[4] == 0xFF)
        {
            adac_write(0xFF);				// LCD full brighness
            start_record_play = 1;		// recording or playing can now start
            lcd_cursorBlink(1);				// blinking cursor on
        }
        else
        {
            adac_write(0x80);
        }

        // Joystick Left, Go Live
        //************************************************
        if (readADAC[2] == 0xFF)
        {
            goLive = 0;

            lcd_cursorShow(0);				// cursor on
            lcd_cursorBlink(0);				// blinking cursor off

            recordPlay = 3;						// playing flag set
            start_record_play = 0;			// don't start playing
        }

        // Joystick Right, Back to main menu
        //************************************************
        if (readADAC[2] == 0x00)
        {
            lcd_cursorShow(0);				// cursor on
            lcd_cursorBlink(0);				// blinking cursor off

            recordPlay = 4;						// playing flag set
            start_record_play = 0;			// don't start playing
        }

        //************************************************
        /* In this section:
         * 				- Record data
         * 				- Save recorded data to flash
         * 				- retreive recorded data from flash
         * 				- Playback data with pwm		*/
        //************************************************
        //Recording flag was set - Record and save to flash
        //************************************************
        if ((recordPlay == 1) && (start_record_play == 1))
        {
            __delay_cycles(100);

            // reset flags
            recordPlay = 0;
            start_record_play = 0;

            // re-initialize timer and pwm
            configure_Timer_PWM_Init();

            /* Record voice from microphone and save in flash */
            for (sector = 0; sector < M25P16_SECTOR_COUNT; sector++)
            {
                // Keep recording for preset time
                if (recordTimeElapsed == 0)
                {
                    // if recording time has not elapsed
                    record_data();

                    __delay_cycles(100);

                    // Save recorded data to flash
                    save_data_to_flash(sector);

                    // keeps track of sector written
                    sectors_Written = sector;
                }
                else
                {
                    lcd_cursorShow(1);				// cursor on
                    lcd_cursorBlink(0);				// blinking cursor off
                }
            }
        }

        //palying flag was set - Retrieve data from flash and playback
        //************************************************
        if ((recordPlay == 2) && (start_record_play == 1))
        {
            __delay_cycles(1600);

            // reset flags
            recordPlay = 0;
            start_record_play = 0;

            /* Retrieve data from flash and playback using pwm */
            for (sector = 0; sector < sectors_Written; sector++)
            {
                if (sectors_Written > 31)
                {
                    return 1;
                }
                // retreived data from flash sector by sector
                retreive_data_from_flash(sector);
                // playback each data retreived from sector
                audio_playBuffer();
            }
        }

        //Go Live - Records and playback  immediately
        //************************************************
        if ((recordPlay == 3) && (start_record_play == 1))
        {

            if (goLive == 0)
            {
                // show Go Live on LCD
                lcd_main_menu(1);
                // dim  LCD
                adac_write(0x80);

                goLive++;
            }

            // Continue Love broadcasting
            verify_recording_playback();
        }

        // Go back to the main menu
        //************************************************
        if ((recordPlay == 4) && (start_record_play == 1))
        {
            // reset flags
            recordPlay = 0;
            start_record_play = 0;

            // go back to LCD main menu display
            lcd_main_menu(0);
        }
    } // end while loop
} // end main

// Interrupt routine to generate Sampling rate of 20KHZ
//************************************************
// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    CCR0 = Resolution;                        // Add Offset to CCR0

    // increment counter
    sampleRateCount++;

    // Sampling rate of 20KHZ
    if (sampleRateCount == 3)
    {
        // reset counter
        sampleRateCount = 0;
        // set flag
        sampleRateFlag = 1;
    }
    else
    {
        sampleRateFlag = 0;
    }
}

// Interrupt routine to generate 2 seconds duration of recording
//************************************************
// Timer_A3 Interrupt Vector (TA0IV) handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
    switch (TA0IV)
    {
    case 2:  // TACCR1 CCIFG
        break;

    case 4:  // TACCR2 CCIFG
        break;

    case 10:  // TAIFG - overflow

        // time(s) = numberOfCounts/frequencyOfClock
        /*******************************************/

        // increment (ms) counter
        timerCount_ms++;

        // 1ms seconds reached?
        if (timerCount_ms == 62)
        {
            // reset (ms) counter
            timerCount_ms = 0;
            //increment (sec) counter
            timerCount_sec++;
            // (3)sec reached?
            if (timerCount_sec == 2000)
            {
                // reset (1sec) counter
                timerCount_sec = 0;
                // set flag
                recordTimeElapsed = 1;
            }
            else
            {
                recordTimeElapsed = 0;
            }
        }
        break;
    }
}
