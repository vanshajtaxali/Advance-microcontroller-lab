/*
 * spi.c
 *
 *  Created on: 16.07.2016
 *      Author: Hassan Gamaleldin
 */

#include <msp430g2553.h>
#include "spi.h"
//#include "isr.h"


//int writingFlag = 0;

// Set the USCI-machine to SPI and switch the 74HCT4066 (1 Pt.)
void spi_init(void) {
	// enable the analog switch
	P1DIR |= BIT3;
	P1OUT &= (~BIT3);

	/*
	 * P3OOUT before P3DIR
	 * The reason for this is that if we were to first set the direction, the output register might have been set to ‘0’.
	 * In the time between the output register being 0 and being set to 1, we will cause an undesirable and momentary glitch on the line.
	 * The line might go From High Impedance (since by default the pins are in high impedance), to ‘0’ and then to ‘1’.
	 * The change from ‘0’ to ‘1’ is mostly harmless, but can sometimes cause issues with devices.
	 */
	P3OUT |= BIT3 | BIT4 | BIT5;
	P3DIR |= BIT3 | BIT4 | BIT5;

	P1SEL = BIT5 | BIT6 | BIT7;
	P1SEL2 = BIT5 | BIT6 | BIT7;

	UCB0CTL1 = UCSWRST;
	//UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
	//UCB0CTL0 |= UCCKPH + UCCKPL + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
	//UCB0CTL0 |= UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
	UCB0CTL1 |= UCSSEL_2; // SMCLK   //UCSSEL1 + UCSSEL0;  //SMCLK     //
	UCB0BR0 |= 0x02; // /2
	UCB0BR1 = 0; //
	UCA0MCTL = 0; // No modulation
	UCB0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**

	IE2 |= UCB0RXIE;
}

// Read <length> bytes into <rxData> (2 Pts.)
void spi_read(unsigned char length, unsigned char * rxData) {
	if (length == 0) {
		return;
	}

	spi_dataIBuffer = rxData;
	spi_dataLength = length;
	rwFlag = 1;

	while (!(IFG2 & UCB0TXIFG))
		; // USCI_B0 TX buffer ready?
	UCB0TXBUF = 0x00;                     // Transmit first character

	//IE2 |= UCB0RXIE;
	__bis_SR_register(LPM0_bits + GIE);       // CPU off, enable interrupts
}

// Write <length> bytes from <txData> (2 Pts.)
void spi_write(unsigned char length, unsigned char * txData) {

	if (length == 0) {
		return;
	}

	spi_dataOBuffer = txData;
	spi_dataLength = length;
	rwFlag = 0;

	while (!(IFG2 & UCB0TXIFG))
		; // USCI_B0 TX buffer ready?

	UCB0TXBUF = *spi_dataOBuffer;                    // Transmit first character

	__bis_SR_register(LPM0_bits + GIE);

}

// Returns 1 if the spi is still busy or 0 if not.
// Note: this is optional. You will most likely need this, but you don't have
// to implement or use this.
// (You probably want to record new data while still writing the old one,
// so you most likely don't want to go into low power mode while waiting for
// the SPI-write to finish.)
unsigned char spi_busy(void) {

	return 0;
}



