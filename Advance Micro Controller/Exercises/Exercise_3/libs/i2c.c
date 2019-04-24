/*
 * i2c.c
 *
 *  Created on: 21.05.2015
 *      Author: <fill in your name here>
 */

#include "i2c.h"

/*
 *
 * 	Here be dragons.
 *
 */

int flagToStop = 0;
int NStopFlag = 0;			//IF this flag is on, don't send stop signal
unsigned int TXIndex = 0;                // index for writing to the TX buffer
unsigned char *PTxData;                     // Pointer to TX data
unsigned char *PRxData;                     // Pointer to RX data
unsigned char TXByteCtr, RXByteCtr, RX = 0;

void i2c_init(unsigned char addr, unsigned char speed) {
	if (speed == 1) {
		UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
	} else if (speed == 4) {
		UCB0BR0 = 3;                             // fSCL = SMCLK/3 = ~400kHz
	}
	UCB0BR1 = 0;
	UCB0I2CSA = addr;                         // Slave Address is 048h
}

unsigned char i2c_write(unsigned char length, unsigned char * txData,
		unsigned char stop) {

	Setup_TX();

	if (stop > 0) {
		NStopFlag = 0;
	} else {
		NStopFlag = 1;
	}
	PTxData = txData;
	TXByteCtr = length;                  // Load TX byte counter

	// Before writing, you should always check if the last STOP-condition has already been sent.
	while (UCB0CTL1 & UCTXSTP)
		;
	UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

	//check for ack, and decide whether to return 0 or 1
	int ack = 0;
	if (UCB0STAT & UCNACKIFG) {
		ack = 1;
	}
	// Enter LPM0, keep general interrupts enabled
	__bis_SR_register(LPM0_bits | GIE);
	return ack;
}
void i2c_read(unsigned char length, unsigned char * rxData) {
	Setup_RX();

	PRxData = rxData;
	RXByteCtr = length;              		// Load RX byte counter

	// Before writing, you should always check if the last STOP-condition has already been sent.
	while (UCB0CTL1 & UCTXSTP)
		;
	UCB0CTL1 |= UCTXSTT;                    // I2C start condition

	if (length == 1) {
		// Todo: If you only want to receive one byte, you instantly have to write a STOP-condition
		// after the START-condition got sent.
		UCB0CTL1 |= UCTXSTP;
	}
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
}

void Setup_TX(void) {
	__disable_interrupt();
	RX = 0;
	IE2 &= ~UCB0RXIE;
	//while (UCB0CTL1 & UCTXSTP);               // Ensure stop condition got sent// Disable RX interrupt
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
	IE2 |= UCB0TXIE;                          // Enable TX interrupt
}
void Setup_RX(void) {
	__disable_interrupt();
	RX = 1;
	IE2 &= ~UCB0TXIE;
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
	IE2 |= UCB0RXIE;                          // Enable RX interrupt
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void) {
	if (RX == 1) {                              // Master Recieve?
		//if (RXByteCtr > 0) {

		RXByteCtr--;                            // Decrement RX byte counter
		if (RXByteCtr) {
			PRxData[TXIndex++] = UCB0RXBUF; // Move RX data to address PRxData
		} else {
			PRxData[TXIndex] = UCB0RXBUF;   // Move final RX data to PRxData
			TXIndex = 0;
			__bic_SR_register_on_exit(LPM0_bits);               // Exit LPM0
		}
		if (RXByteCtr == 1) {
			UCB0CTL1 |= UCTXSTP;      				  // stop condition
		}

		/*
		 * In the data sheet, to receive only one more byte, you have to set UCTXSTP.
		 * Why the author of the example from which this code was originally taken didn't account
		 * for it is beyond me :(
		 */

		//} else {
		//	UCB0CTL1 |= UCTXSTP;
		//	__bic_SR_register_on_exit(LPM0_bits);
		//}
	}

	else {                                     // Master Transmit

		if (TXByteCtr)                        // Check TX byte counter
		{
			//UCB0TXBUF = MSData++;                   // Load TX buffer
			UCB0TXBUF = PTxData[TXIndex++];
			TXByteCtr--;                            // Decrement TX byte counter

		} else {
			//UCB0TXBUF = PTxData[TXIndex];				//Move final PRxData to  TX
			TXIndex = 0;
			if (NStopFlag == 1) {
				IFG2 &= ~UCB0TXIFG;                 // Clear USCI_B0 TX int flag
				__bic_SR_register_on_exit(LPM0_bits);               // Exit LPM0
			} else {
				UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
				IFG2 &= ~UCB0TXIFG;                 // Clear USCI_B0 TX int flag
				__bic_SR_register_on_exit(LPM0_bits);               // Exit LPM0
			}
		}
	}
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void) {
	// Todo: Check for NACKs

}

/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
