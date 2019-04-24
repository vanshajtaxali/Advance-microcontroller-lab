/*
 * spi.h
 *
 *  Created on: 18.06.2015
 *      Author: sester
 */

#ifndef LIBS_SPI_H_
#define LIBS_SPI_H_


unsigned char* spi_dataIBuffer;
unsigned char* spi_dataOBuffer;
unsigned char spi_dataLength;
int rwFlag;

// Set the USCI-machine to SPI and switch the 74HCT4066 (1 Pt.)
void spi_init(void);

// Read <length> bytes into <rxData> (2 Pts.)
void spi_read(unsigned char length, unsigned char * rxData);

// Write <length> bytes from <txData> (2 Pts.)
void spi_write(unsigned char length, unsigned char * txData);

// Returns 1 if the spi is still busy or 0 if not.
// Note: this is optional. You will most likely need this, but you don't have
// to implement or use this.
// (You probably want to record new data while still writing the old one,
// so you most likely don't want to go into low power mode while waiting for
// the SPI-write to finish.)
unsigned char spi_busy(void);

#endif /* LIBS_SPI_H_ */
