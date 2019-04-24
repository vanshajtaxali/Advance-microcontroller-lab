/*
 * adac.h
 *
 *  Created on: 21.05.2015
 *      Author: Sebastian Sester (sebastian.sester@imtek.de)
 */

#ifndef EXERCISE_3_LIBS_ADAC_H_
#define EXERCISE_3_LIBS_ADAC_H_

// All functions return 0 if everything went fine
// and anything but 0 if not.

// Initialize the ADC / DAC
unsigned char adac_init(void);
// Read all ADC-values and write it into the passed values-array.
// (Important: always pass an array of size four (at least).)
unsigned char adac_read(unsigned char * values);
// Write a certain value to the DAC.
unsigned char adac_write(unsigned char value);

#endif /* EXERCISE_3_LIBS_ADAC_H_ */
