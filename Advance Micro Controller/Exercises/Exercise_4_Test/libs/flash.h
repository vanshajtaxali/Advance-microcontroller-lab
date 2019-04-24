/*
 * flash.h
 *
 *  Created on: 18.06.2015
 *      Author: Sebastian Sester
 */

#ifndef LIBS_FLASH_H_
#define LIBS_FLASH_H_


// Initialise the flash chip (in case you need it, else leave this function
// empty).
void flash_init(void);

// Read <length> bytes into <rxData> starting from address <address>
void flash_read(long int address, unsigned char length, unsigned char * rxData);

// Write <length> bytes from <txData>, starting at address <address>
void flash_write(long int address, unsigned char length, unsigned char * txData);

// Returns 1 if the flash is busy or 0 if not.
// Note: this is optional. You will probably need this, but you don't have to
// implement this if you solve it differently.
unsigned char flash_busy(void);

#endif /* LIBS_FLASH_H_ */
