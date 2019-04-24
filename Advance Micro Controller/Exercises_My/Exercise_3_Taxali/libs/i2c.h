/***************************************************************************//**
 * @file    i2c.h
 * @author  <Vanshaj Taxali>
 * @date    <26/05/2018>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef EXERCISE_LIBS_I2C_H_
#define EXERCISE_LIBS_I2C_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <msp430g2553.h>

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/
#define I2C_SPI     BIT3
#define XSCL        BIT6
#define XSDA        BIT7

unsigned char SMCLK_divider;                // holds clock divider value
unsigned char *ptrRxData;                   // Pointer to RX data
unsigned char *ptrTxData;                   // Pointer to TX data
unsigned char rxByteCtr;                // RX byte counter
unsigned char txByteCtr, dummyCtr;          // TX byte counter
unsigned char _stop;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

// Initialize the I2C state machine. The speed should be 100 kBit/s.

void i2c_init (unsigned char addr, unsigned char speed);

// Write a sequence of <length> characters from the pointer <txData>.
// Return 0 if the sequence was acknowledged, 1 if not. Also stop transmitting further bytes upon a missing acknowledge.

unsigned char i2c_write(unsigned char length, unsigned char * txData, unsigned char stop);


void i2c_read(unsigned char length, unsigned char * rxData);

#endif /* EXERCISE_LIBS_I2C_H_ */
