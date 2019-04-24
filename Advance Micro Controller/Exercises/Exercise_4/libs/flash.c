/*
 * spi.c
 *
 *  Created on: 16.07.2016
 *      Author: Hassan Gamaleldin
 */

#include <msp430g2553.h>
#include "flash.h"
#include "spi.h"
//#include "stdlib.h"
// Initialise the flash chip (in case you need it, else leave this function
// empty).
void flash_init(void) {

	spi_init();

	//The following code is to test reading without making sure writing works
	//unsigned char bytes_to_write[1];
	//unsigned char bytes_to_read[3];
	//bytes_to_write[0] = 0x
	//bytes_to_write[0] = 0x9F;

	//P3OUT &= (~BIT4); // Select Device
	//spi_write(1, bytes_to_write);

	//spi_read(3, bytes_to_read);

	//P3OUT |= (BIT4); // Unselect Device
}

// Read <length> bytes into <rxData> starting from address <address>
void flash_read(long int address, unsigned char length, unsigned char * rxData) {
	unsigned char bytes_to_write[4];
	bytes_to_write[0] = 0x03;
	bytes_to_write[1] = (address >> 16) & 0xFF;
	bytes_to_write[2] = (address >> 8) & 0xFF;
	bytes_to_write[3] = address & 0xFF; //get only the least 8 bits

	//int sizeofBuffer = length;
//	unsigned char* dummyBytes = malloc(
	//				sizeofBuffer * sizeof(unsigned char)); // array to hold the result

	//unsigned char bytes_to_read[4];
	P3OUT &= (~BIT4); // Select Device
	spi_write(4, bytes_to_write);

	spi_read(length, rxData);

	P3OUT |= (BIT4); // Unselect Device

}

// Write <length> bytes from <txData>, starting at address <address>
void flash_write(long int address, unsigned char length, unsigned char * txData) {
	//int sizeofBuffer = length + 4;
	//unsigned char* bytes_to_write = malloc(
	//		sizeofBuffer * sizeof(unsigned char)); // array to hold the result
	//unsigned char* bytes_to_read = malloc(
	//			sizeofBuffer * sizeof(unsigned char)); // array to hold the result
	unsigned char bytes_to_write[4];
	bytes_to_write[0] = 0x02;
	bytes_to_write[1] = (address >> 16) & 0xFF;
	bytes_to_write[2] = (address >> 8) & 0xFF;
	bytes_to_write[3] = address & 0xFF; //get only the least 8 bits

	unsigned char control_bytes[1];
	//unsigned char dummyByte[1];

	//int i;

	//for (i = 0; i < length; ++i) {
	//	bytes_to_write[i + 4] = txData[i];
	//}

	//set WREN
	control_bytes[0] = 0x06;
	P3OUT &= (~BIT4); // Select Device
	spi_write(1, control_bytes);
	P3OUT |= (BIT4); // Unselect Device

	P3OUT &= (~BIT4); // Select Device
	spi_write(4, bytes_to_write);
	spi_write(length, txData);
	P3OUT |= (BIT4); // Unselect Device

	//reset WREN
	control_bytes[0] = 0x04;
	P3OUT &= (~BIT4); // Select Device
	spi_write(1, control_bytes);
	P3OUT |= (BIT4); // Unselect Device
}

// Returns 1 if the flash is busy or 0 if not.
// Note: this is optional. You will probably need this, but you don't have to
// implement this if you solve it differently.
unsigned char flash_busy(void) {
	return 0;
}
