/*
 * i2c.c
 *
 *  Created on: 21.05.2015
 *      Author: Jochen Kempfle
 */

#include "i2c.h"

#include <msp430g2553.h>
//#include "lcd.h"
//#include "isr.h"

// Initialize the I2C state machine. Speed can be either 1
// (normal mode, 100 kbit/s) or 4 (Fast Mode, 400 kbit/s).
void i2c_init (unsigned char addr, unsigned char speed)
{
	// enable the analog switch
	P1DIR |= BIT3;
	P1OUT |= BIT3;
	//P1REN |= BIT3;
	__disable_interrupt();
	// enter the software reset mode by setting the UCSWRT bit
	UCB0CTL1 = UCSWRST;
	// select I2C, master and synchronous mode
	UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;
	// use SMCLK as clock source
	UCB0CTL1 |= UCSSEL_2;
	// SMCLK ~ 16 MHz => SMCLK/160 ~ 100 kHz, SMCLK/40 ~ 400 kHz
	UCB0BR0 = (speed == 1 ? 160 : 40);
	UCB0BR1 = 0;
	// Set slave address
	UCB0I2CSA = addr;
	// assign I2C pins to USCI_B0
	P1SEL |= BIT6 | BIT7;
	P1SEL2 |= BIT6 | BIT7;
	// leave software reset mode by clearing the UCSWRT bit
	UCB0CTL1 &= ~UCSWRST;

	// while (UCB0STAT & UCBBUSY);
	// enable TX, RX and NACK interrupt by setting UCB0TXIE, UCB0RXIE, UCNACKIE and GIE
	IE2 |= UCB0TXIE | UCB0RXIE;
	UCB0I2CIE |= UCNACKIE;
	// __bis_SR_register(GIE);
	__enable_interrupt();
}

// Write a sequence of <length> characters from the pointer <txData>.
// Return 0 if acknowledged, 1 if not. Also stop transmitting further bytes upon a missing acknowledge.
// Only send a stop condition if <stop> is not 0
unsigned char i2c_write(unsigned char length, unsigned char* txData, unsigned char stop)
{
	if (length == 0)
	{
		return 1;
	}

	i2c_dataBuffer = txData;
	i2c_dataLength = length;
	i2c_nackReceived = 0;

	while (UCB0CTL1 & UCTXSTP);
	// to be sure, enable interrupts as the device otherwise sleeps forever
	__enable_interrupt();
	// set I2C TX mode and send start condition
	UCB0CTL1 |= UCTR | UCTXSTT;

	// go in low power mode
	__low_power_mode_0();

	if (stop)
	{
		// send stop condition
		UCB0CTL1 |= UCTXSTP;
	}
	// if the data got not acknowledged, return 1
	if (i2c_nackReceived == 1)
	{
		i2c_nackReceived = 0;
		return 1;
	}

	return 0;
}

// Returns the next <length> characters
void i2c_read(unsigned char length, unsigned char* rxData)
{
	if (length == 0)
	{
		return;
	}

	i2c_dataBuffer = rxData;
	i2c_dataLength = length;

	while (UCB0CTL1 & UCTXSTP);

	// set I2C RX mode
	UCB0CTL1 &= ~UCTR;

	// just to be sure, first enable interrupts as the device otherwise sleeps forever
	__enable_interrupt();

	// if only one byte should be received, the stop condition must be sent while the byte is being received
	if (length == 1)
	{
		// send start condition
		UCB0CTL1 |= UCTXSTT;
		// wait for the start condition to be sent
		while (UCB0CTL1 & UCTXSTT);
		// and immediately set the stop flag (before finishing the receiving of the byte)
		UCB0CTL1 |= UCTXSTP;
	}
	else
	{
		// send start condition
		UCB0CTL1 |= UCTXSTT;
	}

	// go in low power mode
	__low_power_mode_0();
}


#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	// check if we are in TX mode
	if (IFG2 & UCB0TXIFG)
	{
		if (i2c_dataLength > 0)
		{
			UCB0TXBUF = *i2c_dataBuffer;
			++i2c_dataBuffer;
			--i2c_dataLength;
		}
		else
		{
			// clear USCI_B0 TX interrupt flag as there will no more data be written to the buffer
			IFG2 &= ~UCB0TXIFG;
			// exit low power mode
			__low_power_mode_off_on_exit();
		}
	}
	// check if we are in RX mode
	else if (IFG2 & UCB0RXIFG)
	{
		// read the byte and increase the data position
		*i2c_dataBuffer = UCB0RXBUF;
		++i2c_dataBuffer;
		--i2c_dataLength;

		// if the next incoming byte is the last to be received, send stop
		if (i2c_dataLength == 1)
		{
			UCB0CTL1 |= UCTXSTP;
		}
		// no more data to be read -> exit low power mode
		else if (i2c_dataLength == 0)
		{
			__low_power_mode_off_on_exit();
		}
	}
}
/*
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
	// check for NACK
	if (UCB0STAT & UCNACKIFG)
	{
		// send a stop condition if slave sends NACK and clear the NACK flag
		UCB0CTL1 |= UCTXSTP;
		UCB0STAT &= ~UCNACKIFG;
		i2c_nackReceived = 1;
		// exit low power mode
		__low_power_mode_off_on_exit();
	}
}

*/
