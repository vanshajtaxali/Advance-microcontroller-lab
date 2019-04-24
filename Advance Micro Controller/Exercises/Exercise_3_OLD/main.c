/*
 * Exercise sheet 3
 *
 *  Created on: 04.06.2015
 *      Author: <fill in your name here>
 */
#define NO_TEMPLATE_UART
#include <templateEMP.h>
#include "libs/adac.h"
#include "libs/mma.h"
#include "libs/LCD.h"
#include "libs/i2c.h"//remove later

void Initialize(void);
void GoToSleep(void);
void Run(void);
//void GameLoop(void);
int main(void) {
	initMSP();
	// TODO (sesters): Add your initialization here.

	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	P1SEL2 |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0

	//To Enable our ports on the I2C Bus on the extension board
	P1DIR |= (BIT3);

	P1OUT |= BIT3;

	//const unsigned char adac_control_byte = 68;//from the PCF8591 datasheet
	//unsigned char bytes_to_write[2];
	//bytes_to_write[0] = adac_control_byte;

	lcd_init();

	__delay_cycles(500000);	//500ms delay. There are some overheads, so, not quite exact.

	Initialize();

	__delay_cycles(500000);	//500ms delay. There are some overheads, so, not quite exact.

	GoToSleep();

	lcd_clear();

	__delay_cycles(500000);	//500ms delay. There are some overheads, so, not quite exact.

	Run();
}

void Initialize(void) {
	unsigned char status = 0x00;
	status |= mma_init();

	lcd_putText("ST will begin...");
	__delay_cycles(2000000);	//2s delay. There are some overheads, so, not quite exact.

	lcd_clear();

	status |= mma_selftest();

	if (status != 0) {
		lcd_putText("Error self testing mma");
		__delay_cycles(2000000);	//2s delay. There are some overheads, so, not quite exact.
		status = 0x00;
		lcd_clear();
	}

	lcd_clear();
	//since I can't make it work, I won't loop it till it succeeds

	status |= mma_enableTapInterrupt();
	if (status != 0) {
		lcd_putText("Error initializing mma");
		__delay_cycles(2000000);	//2s delay. There are some overheads, so, not quite exact.
		status = 0x00;
		lcd_clear();
	}

	//For the double tap ISR
	P1DIR &= (~BIT4);
	P1REN |= BIT4;					// Enable internal pull-up/down resistors
	P1OUT |= BIT4;					//Select pull-up mode for P1.4
	P1IE |= BIT4;					// P1.4 interrupt enabled
	P1IES |= BIT4;				// P1.4 Lo/hi edge
	P1IFG &= ~BIT4;					// P1.4 IFG cleared
}

void GoToSleep(void) {
	lcd_putText("Sleeping...");

	__bis_SR_register(LPM4_bits | GIE);
}

void Run(void) {
	mma_disableTapInterrupt();
	unsigned char status = 0x00;
	unsigned char val = 0xFF;
	unsigned char bytes_to_read[5];
	//status |= mma_init();
	status |= mma_read();

	adac_init();
	status |= adac_write(val);

	lcd_putText("Running... ");
	__delay_cycles(1000000);//1s delay. There are some overheads, so, not quite exact.

	lcd_clear();

	while (1) {
		// TODO (sesters): Implement exercise 3 here.

		//was added to test the i2c, and keeping it, to know when the code is not working
		status |= adac_read(bytes_to_read);

		if (status != 0) {
			lcd_putChar('*');
		}

		//was added to test the i2c, and keeping it, to know when the code is not working
		val = bytes_to_read[1];
		status |= adac_write(val);

		if (status != 0) {
			lcd_putChar('*');
		}
	}
}
// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
	_BIC_SR(LPM4_EXIT);

	P1IE &= (~BIT4);
	P1IFG &= ~BIT4;

	// P1.3 IFG cleared
}
