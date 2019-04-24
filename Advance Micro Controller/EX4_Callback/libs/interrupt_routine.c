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


}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{

    rx_callback();

	// spi interrupt was received

}


