/***************************************************************************//**
 * @file    I2C.h
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <14-06-2018>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef _LIBS_I2C_H_
#define _LIBS_I2C_H_

#define I2C_SPI		BIT3
#define XSCL		BIT6
#define XSDA		BIT7

//static volatile unsigned char* i2c_dataBuffer;
//static volatile unsigned char i2c_dataLength;
//static volatile unsigned char i2c_nackReceived;
//
//_Bool IAM_i2c;
//_Bool IAM_spi;

// Initialize the I2C state machine. Speed can be either 1
// (normal mode, 100 kbit/s) or 4 (Fast Mode, 400 kbit/s).
void i2c_init (unsigned char addr, unsigned char speed);
// Start an I2C-connection (send the start byte and the address).
// Return anything but 0 if there was an error.
unsigned char i2c_start();
// Write a sequence of <length> characters from the pointer <txData>.
// Return 0 if acknowledged, 1 if not. Also stop transmitting further bytes upon a missing acknowledge.
// Only send a stop condition if <stop> is not 0
unsigned char i2c_write(unsigned char length, unsigned char * txData, unsigned char stop);
// Returns the next <length> characters
void i2c_read(unsigned char length, unsigned char * rxData);

#endif /* _LIBS_I2C_H_ */
