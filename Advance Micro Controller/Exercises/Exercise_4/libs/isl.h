/*
 * isl.h
 *
 *  Created on: 18.06.2015
 *      Author: sester
 */
//#include <msp430g2553.h>

#ifndef LIBS_ISL_H_
#define LIBS_ISL_H_

// As always: return 0 if all went fine, 1 if not.
// Initialise the isl chip (and set the wiper to 50 %)
unsigned char isl_init(void);
// Set the wiper to <wiperValue>.
unsigned char isl_set(unsigned char wiperValue);

#endif /* LIBS_ISL_H_ */
