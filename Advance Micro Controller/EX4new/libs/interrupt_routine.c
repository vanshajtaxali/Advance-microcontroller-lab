/***************************************************************************//**
 * @file    interrupt_routine.c
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

void (*tx_callback)(void);
void (*rx_callback)(void);

unsigned char dummy;

void isr_register_txCb(void (*user_callback)(void))
{
    tx_callback = user_callback;
}

void isr_register_rxCb(void (*user_callback)(void))
{
    rx_callback = user_callback;
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
    tx_callback();
//    if(IAM_spi == 1)
//   {
//    if(IAM_spiTX == 1)  // Transmitting
//            {
//                if(spi_txdataLength > 0)
//                {
//                    while(!(UC0IFG & UCB0TXIFG));       // wait till USCI_A0 TX buffer ready?
//                    UCB0TXBUF = *spi_txdataBuffer;      // transmit value
//                    spi_txdataBuffer++;                 // increment data buffer
//                    spi_txdataLength--;                 // increment counter
//
//
//                } else
//                {
//                    // clear interrupt flag
//                    UC0IFG &= ~UCB0RXIFG;
//                    // exit low power mode
//                   // __low_power_mode_off_on_exit();
//                }
//            }
//    }
//		// i2c interrupt was received
//	if(IAM_i2c)
//	{
//		IAM_spi = 0;
//
//		// check if we are in TX mode
//		if (IFG2 & UCB0TXIFG)
//		{
//			if (i2c_dataLength > 0)
//			{
//				UCB0TXBUF = *i2c_dataBuffer;
//				++i2c_dataBuffer;
//				--i2c_dataLength;
//			}
//			else
//			{
//				// clear USCI_B0 TX interrupt flag as there will no more data be written to the buffer
//				IFG2 &= ~UCB0TXIFG;
//				// exit low power mode
//				__low_power_mode_off_on_exit();
//			}
//		}
//		// check if we are in RX mode
//		else if (IFG2 & UCB0RXIFG)
//		{
//			// read the byte and increase the data position
//			*i2c_dataBuffer = UCB0RXBUF;
//			++i2c_dataBuffer;
//			--i2c_dataLength;
//
//			// if the next incoming byte is the last to be received, send stop
//			if (i2c_dataLength == 1)
//			{
//				UCB0CTL1 |= UCTXSTP;
//			}
//			// no more data to be read -> exit low power mode
//			else if (i2c_dataLength == 0)
//			{
//				__low_power_mode_off_on_exit();
//			}
//		}
//	}
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
    //IFG2 &= ~UCB0RXIFG;

	// i2c interrupt was received
	if(IAM_i2c == 1)
	{
		IAM_spi = 0;

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

	// spi interrupt was received
	if(IAM_spi == 1)
	{

		if(IAM_spiRX == 1)	// receiving
		{
			if (spi_rxdataLength == 1) // this is last byte
			{
				*spi_rxdataBuffer = UCB0RXBUF;
				__low_power_mode_off_on_exit();
			}
			else
			{
				while(!(UC0IFG & UCB0TXIFG));		// wait till USCI_A0 TX buffer ready?
				*spi_rxdataBuffer = UCB0RXBUF;			// receive value in rxData
				++spi_rxdataBuffer;						// increment data buffer
				--spi_rxdataLength;						// increment buffer

				// send a dummy byte to read next byte
				UCB0TXBUF = 0x00;						// dummy byte

			}
		}
	}
}


