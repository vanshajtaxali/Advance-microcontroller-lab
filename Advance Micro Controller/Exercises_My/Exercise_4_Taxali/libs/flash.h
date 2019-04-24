/***************************************************************************//**
 * @file    flash.h
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <14-06-18>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_FLASH_H_
#define LIBS_FLASH_H_

#include "./spi.h"

#define    WRITE_EN         0x06       // WREN: Write Enable
#define    WRITE_DISABLE    0x04       // WRDI: Write Disable
#define    READ_ID          0x9F       // RDID: Read Identification
#define    READ_STATUS_REG  0x05       // RDSR: Read Status Register
#define    WRITE_STATUS_REG 0x01       // WRSR: Write Status Register
#define    READ_DATA        0x03       // READ: Read Data Bytes
#define    FAST_READ_DATA   0x0B       // FAST_READ: Read Data Bytes at Higher Speed
#define    PAGE_PRO         0x02       // PP : Page Program
#define    SECTOR_ERASE     0xD8       // SE : Sector Erase
#define    BULK_ERASE       0xC7       // BE : Bulk Erase
#define    POWER_DOWN       0xB9       // DP : Deep Power-down
#define    M25P16_RES       0xAB       // RES: Release from Deep Power-down



#define M25P16_SREG_WRITE_IN_PROGRESS(SREG)     ((SREG) & (1 << 0))

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

// Write Enable/Disable.
void flash_write_enable(_Bool op);

// erase throughout the entire memory -Bulk Erase
unsigned char flash_erase(void);

#endif /* LIBS_FLASH_H_ */
