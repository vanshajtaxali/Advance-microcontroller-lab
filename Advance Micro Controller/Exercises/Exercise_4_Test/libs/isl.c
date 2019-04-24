/*
 * isl.c
 *
 *  Created on: 16.07.2016
 *      Author: Hassan Gamaleldin
 */

//#include <msp430g2553.h>
#include "isl.h"
#include "i2c.h"

// As always: return 0 if all went fine, 1 if not.
// Initialise the isl chip (and set the wiper to 50 %)
unsigned char isl_init(void) {
	unsigned char isl_addr = 40; //MSB = 0, then address for isl = 0101000
	i2c_init(isl_addr, 1);

	unsigned char bytes_to_write[2];

	bytes_to_write[0] = 0x00; //Address of the IVR & WR registers. Set it to 0x80 to not write to IVR.
	bytes_to_write[1] = 0x80; //set wiper to 50%. 50% of 255 is 127.5, so 128 will be set. Which is 0x80.

	return i2c_write(2, bytes_to_write, 1);;
}

// Set the wiper to <wiperValue>.
unsigned char isl_set(unsigned char wiperValue) {
	unsigned char bytes_to_write[2];

	bytes_to_write[0] = 0x00;//Address of the IVR & WR registers. Set it to 0x80 to not write to IVR.
	bytes_to_write[1] = wiperValue;

	return i2c_write(2, bytes_to_write, 1);;
}
