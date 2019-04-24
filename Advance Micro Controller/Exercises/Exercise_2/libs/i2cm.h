/*
 * i2cm.h
 *
 *  Created on: 21.05.2015
 *      Author: Sebastian Sester (sebastian.sester@imtek.de)
 */
#include <msp430g2553.h>
#ifndef EXERCISE_2_GIVEN_LIBS_I2CM_H_
#define EXERCISE_2_GIVEN_LIBS_I2CM_H_

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

void i2c_start(void);

void i2c_stop(void);

//This function will pull the scl high to allow the sda bit to be written, and then pull it down again. The period is 10 us to achieve 100Kb/s
void trigger_scl(void);

//the fixed delay that this module will call whenever it wants to delay fixed time. Here we set it to 5us to achieve 100Kb/s
void i2c_delay(void);

#endif /* EXERCISE_2_GIVEN_LIBS_I2CM_H_ */
