/*
 * mma.h
 *
 *  Created on: 04.06.2015
 *      Author: Sebastian Sester (sebastian.sester@imtek.de)
 */

#ifndef EXERCISE_3_LIBS_MMA_H_
#define EXERCISE_3_LIBS_MMA_H_

// All functions return 0 if everything went fine
// and anything but 0 if not.

// Initialize the MMA with 8 bit resolution and 4G measurement range
unsigned char mma_init(void);

// Change the measurement range. (0: 2g, 1: 4g, >1: 8g)
unsigned char mma_setRange(unsigned char range);
// Change the resolution (0: 8 Bit, >= 1: 14 Bit)
unsigned char mma_setResolution(unsigned char resolution);

// Run a self-test on the MMA, verifying that all three axis and all three
// measurement ranges are working.
unsigned char mma_selftest(void);

// Set up the double tap interrupt on the MMA (do not set up the interrupt on
// the MSP in this function!). This means that the MMA should change the INT1-
// pin whenever a double tap is detected. You may freely choose the axis on
// which the tap has to be received. (You should put a comment in your code,
// which axis you chose, though).
unsigned char mma_enableTapInterrupt(void);
// Disable the double-tap-interrupt on the MMA.
unsigned char mma_disableTapInterrupt(void);

// Read the values of all three axis from the chip and store the values
// internally. Take the requested resolution into account.
unsigned char mma_read(void);

// Important notice: The X/Y-direction printed on the board are wrong:
// X is the real Y and Y is the real X. Sorry for the confusion. :-(

// Return the appropriate 8 bit values
// If the resolution during mma_read was 14 bit, translate the data to 8 bit
signed char mma_get8X(void);
signed char mma_get8Y(void);
signed char mma_get8Z(void);

// Return the appropriate 14 bit values
// If the resolution during mma_read was 8 bit, translate the data to 14 bit
int mma_get14X(void);
int mma_get14Y(void);
int mma_get14Z(void);

// Return the appropriate values in m*s^-2.
double mma_getRealX(void);
double mma_getRealY(void);
double mma_getRealZ(void);

int mma_getDivider(void);//To be used to get the real values

signed char mma_activate(void);
signed char mma_standby(void);

#endif /* EXERCISE_3_LIBS_MMA_H_ */
