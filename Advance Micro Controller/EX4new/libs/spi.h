/***************************************************************************//**
 * @file    spi.h
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <14-06-18>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_SPI_H_
#define LIBS_SPI_H_

#define F_HOLD				BIT3	//P3.3
#define F_CHIP_SELECT		BIT4	//P3.4
#define F_WRITE_PROTECT		BIT5	//P3.5
#define F_SERIAL_CLOCK		BIT5	//P1.5
#define DATA_IN				BIT6	//P1.6
#define DATA_OUT			BIT7	//P1.7
#define MIC					BIT0	//P1.0
#define PWM					BIT6	//P3.6
#define I2C_SPI				BIT3	//P1.3


// Set the USCI-machine to SPI and switch the 74HCT4066 (1 Pt.)
void spi_init(void);;

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

void spi_isr(void);

#endif /* LIBS_SPI_H_ */
