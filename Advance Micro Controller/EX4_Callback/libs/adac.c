/***************************************************************************//**
 * @file    adac.c
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <14-06-2018>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/
#include "./i2c.h"
#include "./adac.h"

// Initialize the ADC / DAC (PCF8591_addr, speed 1=100 Kbit7s )
unsigned char adac_init(void)
{
	i2c_init(PCF8591_addr, 1);			// initialize i2c -  PCF8591_addr, speed

	return 0;
}

// Read all ADC-values and write it into the passed values-array.
// (Important: always pass an array of size four (at least).)
unsigned char adac_read(unsigned char * values)
{
	unsigned char ReadControl[1] = {AUTO_INCREMENT};  // contains Control bytes

	i2c_write(1, ReadControl, 0);

	// read number of bytes - retrieve read data from value[0 to 4]
	i2c_read(5, values);

	return 0;
}

// Write a certain value to the DAC.
unsigned char adac_write(unsigned char value)
{
	unsigned char WriteValues[2] = {};    // data to be transmitted

	WriteValues[0] = ENABLE_DAC;
	WriteValues[1] = value;

	// write data to DAC and then stop i2c communication
	i2c_write(2, WriteValues, 1);

	return 0;
}
