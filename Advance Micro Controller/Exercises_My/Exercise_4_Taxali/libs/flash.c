/***************************************************************************//**
 * @file    flash.c
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <14-06-18>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include <msp430g2553.h>
#include "flash.h"


void flash_init(void)
{
	// initilize spi
	spi_init();
}

// Read <length> bytes into <rxData> starting from address <address>
void flash_read(long int address, unsigned char length, unsigned char * rxData)
{
	unsigned char READ_FROM_FLASH[4] = {0};
//	unsigned char READ_FROM_FLASH[0] = FAST_READ_DATA;

	// select device by driving chip select low
    P3OUT &= ~F_CHIP_SELECT;

	// The command code for READ DATA BYTES is followed by a 3-byte address (A23-A0)
    READ_FROM_FLASH[0] = READ_DATA;
    READ_FROM_FLASH[1] = (address >> 16);
    READ_FROM_FLASH[2] = (address >> 8);
    READ_FROM_FLASH[3] = (address >> 0);
//    READ_FROM_FLASH[4] = 0x00;		// dummy byte (if FAST_READ is to be used)

    // send read instruction and address to read from
	spi_write(4,  READ_FROM_FLASH);

	// Now read from the flash, rxData will contain the data from flash
	spi_read(length, rxData);

	// deselect device by driving chip select high
	P3OUT |= F_CHIP_SELECT;
}

// Write <length> bytes from <txData>, starting at address <address>
void flash_write(long int address, unsigned char length, unsigned char * txData)
{
	unsigned char WRITE_CONTROL[4] = {};
	unsigned char IsFlashBusy = 0;

	// Write Enable.
	flash_write_enable(1);

	// select device by driving chip select low
	P3OUT &= ~F_CHIP_SELECT;

	// The command code for Writing DATA BYTES
	WRITE_CONTROL[0] = (PAGE_PRO);
	WRITE_CONTROL[1] = (address >> 16);
	WRITE_CONTROL[2] = (address >> 8);
	WRITE_CONTROL[3] = (address >> 0);

	// send write instruction and address
	spi_write(4,  WRITE_CONTROL);

	// transmit data
	spi_write(length,  txData);

	// deselect device by driving chip select high
	P3OUT |= F_CHIP_SELECT;

	// wait for write to complete
	do{
		// check if flash is still busy
		IsFlashBusy = flash_busy();
	}while(M25P16_SREG_WRITE_IN_PROGRESS(IsFlashBusy));
}

// Returns 1 if the flash is busy or 0 if not.
// Note: this is optional. You will probably need this, but you don't have to
// implement this if you solve it differently.
unsigned char flash_busy(void)
{
	unsigned char IsBusy = 0;
	unsigned char readReg[1] = {0};
	unsigned char STATUS_REG[1] = {READ_STATUS_REG};

	// select device by driving chip select low
	P3OUT &= ~F_CHIP_SELECT;

	spi_write(1,  STATUS_REG);	// send register to read
	spi_read(1, readReg);		// read the register
	IsBusy = readReg[0];		// get register status

	// deselect device by driving chip select high
	P3OUT |= F_CHIP_SELECT;

	// return status
	return IsBusy;
}


/*
 * Write Enable/Disable.
 * The WRITE ENABLE command sets the write enable latch (WEL) bit.
 * The WRITE DISABLE command resets the write enable latch (WEL) bit.
 */
void flash_write_enable(_Bool op)
{
	unsigned char WRITE_ENA[1] = {0};

	 if (op == 1)
	 {
		 // write enable command
		 WRITE_ENA[0] = WRITE_EN;
	 }
	 else
	 {
		 // write disable command
		 WRITE_ENA[0] = WRITE_DISABLE;
	 }

	// select device by driving chip select low
	P3OUT &= ~F_CHIP_SELECT;

	// send write enable/disable depending on choice
	spi_write(1,  WRITE_ENA);

	// deselect device by driving chip select high
	P3OUT |= F_CHIP_SELECT;
}

// erase throughout the entire memory -Bulk Erase
unsigned char flash_erase(void)
{
	unsigned char ERASE_REG[1] = {BULK_ERASE};
	unsigned char IsFlashBusy = 0;

	// Write Enable.
	flash_write_enable(1);

	// select device by driving chip select low
	 P3OUT &= ~F_CHIP_SELECT;

	 // erase flash
	 spi_write(1,  ERASE_REG);

	// deselect device by driving chip select high
	P3OUT |= F_CHIP_SELECT;

	// wait for erase to complete
	do{
		// check if flash is still busy erasing?
		IsFlashBusy = flash_busy();

	}while(M25P16_SREG_WRITE_IN_PROGRESS(IsFlashBusy));

	// flash write disable
	flash_write_enable(0);

	return 0;
}
