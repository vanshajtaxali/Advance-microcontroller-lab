/*
 * i2c.h
 *
 *  Created on: 04.06.2015
 *      Author: Sebastian Sester (sebastian.sester@imtek.de)
 */

#ifndef EXERCISE_3_LIBS_I2C_H_
#define EXERCISE_3_LIBS_I2C_H_


unsigned char i2c_nackReceived;
unsigned char* i2c_dataBuffer;
unsigned char i2c_dataLength;


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

#endif /* EXERCISE_3_LIBS_I2C_H_ */
