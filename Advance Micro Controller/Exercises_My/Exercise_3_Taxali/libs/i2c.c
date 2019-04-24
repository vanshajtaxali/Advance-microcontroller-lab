/***************************************************************************//**
 * @file    i2c.c
 * @author  <Vanshaj Taxali : Mtr Nr:4558621>
 * @date    <26/05/2018>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./i2c.h"
#include <msp430g2553.h>
/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

// TODO: Implement these functions:
void i2c_init(unsigned char addr, unsigned char speed)
{
    if (speed)
        SMCLK_divider = 160;                //SMCLK/160 = ~100kHz
    else
        SMCLK_divider = 160;            //SMCLK/160 = ~100kHz (stick to ~100kHz)

    //Pin for switching between different input lines of 74HCT4006(Analog switch)
    P1DIR |= I2C_SPI;                       // I2C_SP1 pin directory as output
    P1OUT |= I2C_SPI; // set I2C_SP1 high to switch SCL and SDA lines to XSCL and XSDA

    P1SEL |= (XSCL + XSDA);     // Assign I2C pins to UCB0SCL and UCB0SDA module
    P1SEL2 |= (XSCL + XSDA);    // Assign I2C pins to UCB0SCL and UCB0SDA module
    UCB0CTL1 |= UCSWRST;                    // Enable USCI Software Reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // I2C Master, I2C Mode, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST; // Use USCI 0 Clock Source: 3(SMCLK), keep SW reset
    UCB0BR0 = SMCLK_divider; // fSCL = SMCLK/160 = ~100kHz (USCI B0 Baud Rate 0)
    UCB0BR1 = 0;                          // (USCI B0 Baud Rate 1)  not required
    UCB0I2CSA = addr;                       // Slave Address is 0x3D (00111101)
    UCB0CTL1 &= ~UCSWRST;                   // Clear SW reset, resume operation
    IE2 |= UCB0TXIE + UCB0RXIE;          // Enable TX and RX and Nack interrupts
    UCB0I2CIE |= UCNACKIE;
}

unsigned char i2c_write(unsigned char length, unsigned char * txData,
                        unsigned char stop)
{
    ptrTxData = txData;                     // TX array start address
    txByteCtr = length;                     // Load TX byte counter
    dummyCtr = length;             // Workaround for TX not sending the lastbyte
    _stop = stop;

    // Before writing, you should always check if the last STOP-condition has already been sent.
    while (UCB0CTL1 & UCTXSTP)
        ;

    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, Generate START condition

    // Enter LPM0, keep general interrupts enabled
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until all data is transmitted
                                            // This part was based on a recommendation I could not figure out on my own

    return 0;

}

void i2c_read(unsigned char length, unsigned char * rxData)
{
    ptrRxData = rxData;                     // Start of RX buffer
    rxByteCtr = length;                     // Load RX byte counter
    // Before writing, you should always check if the last STOP-condition has already been sent.
    while (UCB0CTL1 & UCTXSTP)
        ;

    UCB0CTL1 &= ~UCTR;                     // I2C RX mode,

    if (length == 1)
    {
        // Todo: If you only want to receive one byte, you instantly have to write a STOP-condition
        // after the START-condition got sent.
        rxByteCtr = 2;
    }
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition
    // Enter LPM0, keep general interrupts enabled
    __bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/ interrupts -Remain in LPM0 until all data is RX'd

}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
    // TODO: Read RX-Buffer or write TX-Buffer, depending on what you'd like to do.

    // Exit waiting mode after this interrupt, i.e. set the transferFinished variable.
    // TODO: Call this only when necessary
    if (UC0IFG & UCB0TXIFG) // when transmitting
    {
        if (txByteCtr)                         // Check TX byte counter
        {
            UCB0TXBUF = ptrTxData[dummyCtr - txByteCtr];  // Load TX buffer
            txByteCtr--;                       // Decrement TX byte counter
        }
        else
        {
            if (_stop)
            {
                UCB0CTL1 |= UCTXSTP;           // I2C stop condition
            }
            UC0IFG &= ~UCB0TXIFG;              // Clear USCI_B0 TX int flag
            __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
        }
    }
    if (UC0IFG & UCB0RXIFG) //when receiving
    {
        rxByteCtr--;                           // Decrement RX byte counter
        if (rxByteCtr)
        {
            *ptrRxData++ = UCB0RXBUF;       // Move RX data to address ptrRxData

            if (rxByteCtr == 1)
            {              // Only one byte left?
                UCB0CTL1 |= UCTXSTP;           // Generate I2C stop condition
            }
        }
        else
        {
            *ptrRxData = UCB0RXBUF;           // Move final RX data to ptrRxData
            __bic_SR_register_on_exit(LPM0_bits);  // Exit LPM0
        }
    }

}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
    // TODO: Check for NACKs
    if (UCB0STAT & UCNACKIFG)
    {
        UCB0CTL1 |= UCTXSTP;                    // send STOP if slave sends NACK
        UCB0STAT &= ~UCNACKIFG;                 // NAK Condition interrupt Flag
        __bic_SR_register_on_exit(LPM0_bits);   // Exit LPM0
    }
}
