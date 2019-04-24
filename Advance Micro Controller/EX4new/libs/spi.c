/***************************************************************************//**
 * @file    spi.c
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <14-06-18>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/
#include <msp430g2553.h>
#include "interrupt_routine.h"
#include "spi.h"

volatile unsigned char* spi_rxdataBuffer;
volatile unsigned char* spi_txdataBuffer;
volatile unsigned char spi_rxdataLength;
volatile unsigned char spi_txdataLength;
// Flags to identify different intterrupts
volatile unsigned char IAM_i2c;
volatile unsigned char IAM_spi;
volatile unsigned char IAM_spiTX;
volatile unsigned char IAM_spiRX;

// Set the USCI-machine to SPI and switch the 74HCT4066 (1 Pt.)
void spi_init(void)
{
    isr_register_txCb(&spi_isr);

	// set flags identify which interrupt to execute
	IAM_i2c = 0;
	IAM_spi = 1;

	/* configure ports*/
	//Pin for switching between different input lines of 74HCT4006(Analog switch)
	P1DIR |= I2C_SPI;							// I2C_SP1 pin directory as output
	P1OUT &= ~I2C_SPI;

	P3DIR |= F_HOLD + F_WRITE_PROTECT;
	P3OUT |= F_HOLD + F_WRITE_PROTECT;

	// flash chip select pin as output
	P3DIR |= F_CHIP_SELECT;
	// Assign SPI pins to UCA0SIMO and UCA0SOMI and  UCA0CLK module
	P1SEL  |= DATA_IN + DATA_OUT + F_SERIAL_CLOCK;
	P1SEL2 |= DATA_IN + DATA_OUT + F_SERIAL_CLOCK;

	/* Enable USCI Software Reset */
	UCB0CTL1 = UCSWRST;

	/* Initialize all USCI registers with UCSWRST = 1 */
	UCB0CTL0 &= ~UCCKPL;
	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCMODE_0 + UCSYNC;// Clock Polarity, MSB first, 3-pin, 8-bit SPI master
	UCB0CTL1 |= UCSSEL_2;                     	// Clock Source: SMCLK
	UCB0BR0 |= 0x00;                          	// SMCLK/2
	UCB0BR1 = 0x1;                              	// (USCI A0 Baud Rate 1)  not required

	/* Clear SW reset, Initialize USCI state machine */
	UCB0CTL1 &= ~UCSWRST;

	/* Enable USCI0 RX/Global interrupts */
	IFG2 &= ~(UCB0RXIFG | UCB0TXIFG);
	IE2 |= (UCB0RXIE|UCB0TXIE);



	P3OUT |= F_CHIP_SELECT;

	//Wait for slave to initialize
	__delay_cycles(10000);
}

// Read <length> bytes into <rxData> (2 Pts.)
void spi_read(unsigned char length, unsigned char * rxData)
{
	if (length == 0)
		return;

	// set flags identify which interrupt to execute
	/* spi recieve interrupt in this case*/
	IAM_i2c = 0;
	IAM_spi = 1;
	IAM_spiRX = 1;

	spi_rxdataBuffer = rxData;	// spi_rxdataBuffer is a pointer to rxData
	spi_rxdataLength = length;  // spi_rxdataLength remembers the size of data to be read.

	// send a dummy byte to read
	UCB0TXBUF = 0x00;

//	// Enter LPM0, enable interrupts
//	__bis_SR_register(LPM0_bits + GIE);

	// enable interrupts
	__enable_interrupt();
}

// Write <length> bytes from <txData> (2 Pts.)
void spi_write(unsigned char length, unsigned char * txData)
{
	if (length == 0)
		return;

	// set flags identify which interrupt to execute
	/* spi transmit interrupt in this case*/
	IAM_i2c = 0;
	IAM_spi = 1;
	IAM_spiTX = 1;

	spi_txdataBuffer = txData;	// spi_txdataBuffer is a pointer to txData
	spi_txdataLength = length;	// spi_txdataLength remembers the size of data to be transmitter.

	UCB0TXBUF = spi_txdataBuffer[0];	// transmit the first byte
	spi_txdataBuffer++;					// goto next element
	spi_txdataLength--;					// decrease no of elements

	// Enter LPM0, enable interrupts
	//__bis_SR_register(LPM0_bits + GIE);

	// enable interrupts
	__enable_interrupt();

}

unsigned char spi_busy(void)
{
	return (UCBUSY & UCB0STAT);
}

void spi_isr(void)
{
    if(spi_txdataLength > 0)
    {
        while(!(UC0IFG & UCB0TXIFG));       // wait till USCI_A0 TX buffer ready?
        UCB0TXBUF = *spi_txdataBuffer;      // transmit value
        spi_txdataBuffer++;                 // increment data buffer
        spi_txdataLength--;                 // increment counter


    } else
    {
        // clear interrupt flag
        UC0IFG &= ~UCB0RXIFG;
        // exit low power mode
       // __low_power_mode_off_on_exit();
    }
}
