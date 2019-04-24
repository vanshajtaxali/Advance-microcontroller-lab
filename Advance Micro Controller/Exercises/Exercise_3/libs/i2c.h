/*
 * i2c.h
 *
 *  Created on: 04.06.2015
 *      Author: Sebastian Sester (sebastian.sester@imtek.de)
 */
#include <msp430.h>
#ifndef EXERCISE_3_LIBS_I2C_H_
#define EXERCISE_3_LIBS_I2C_H_

// Initialize the I2C state machine. Speed can be either 1
// (normal mode, 100 kbit/s) or 4 (Fast Mode, 400 kbit/s).
// You may stick to 100 kbit/s no matter what <speed> says, for now.
// <addr> is the 7-bit address of the slave (MSB shall always be 0, i.e. "right alignment")
void i2c_init (unsigned char addr, unsigned char speed);
// Write a sequence of <length> characters from the pointer <txData>.
// Return 0 if acknowledged, 1 if not. Also stop transmitting further bytes upon a missing acknowledge.
// Only send a stop condition if <stop> is not 0
unsigned char i2c_write(unsigned char length, unsigned char * txData, unsigned char stop);
// Returns the next <length> characters
void i2c_read(unsigned char length, unsigned char * rxData);

//The following functions prepare the msp for i2c communication
//These functions are a modifed verion of what is in one of the examples provided by TI
//Copyrigths can be found at the bottom of the source file.
void Setup_TX(void);
void Setup_RX(void);

#endif /* EXERCISE_3_LIBS_I2C_H_ */
