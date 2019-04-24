/*
 * adac.c
 *
 *  Created on: 21.05.2015
 *      Author: Hassan Gamaleldin
 */

#include "./adac.h"
#include "i2c.h"

unsigned char adac_init(void)
{
	unsigned char adac_addr = 72;//MSB = 0, then address for ADC = 1001000
	i2c_init(adac_addr, 1);

	return 0;
}

// Read all ADC-values and write it into the passed values-array.
// (Important: always pass an array of size four (at least).)
unsigned char adac_read(unsigned char * values)
{

	/*
	 *	The following line is commented because there is no need for the address byte anymore.
	 */
	//const unsigned char adac_addr = 72;//MSB = 0, then address for ADC = 1001000
	const unsigned char adac_control_byte = 68;//this is 0x44, which will allow the PCF8591 to simultaneously use the DAC ouptut, while reading AIN0, AIN1, AIN2, AIN3



	//unsigned char bytes_to_write[2];
	//bytes_to_write[0] = (adac_addr << 1); //LSB should be 0 for writing
	//bytes_to_write[1] = adac_control_byte;

	//unsigned char ack =  i2c_write(1, bytes_to_write, 1);

	/*
	 * The following lines are edited versions from the 4 lines above,
	 * because there is no need for an address anymore since the init works
	 */
	unsigned char bytes_to_write[1];
	bytes_to_write[0] = adac_control_byte;

	unsigned char ack =  i2c_write(1, bytes_to_write, 1);

	if (ack != 0)
	{
		return 1;
	}

	/*
	 *	The following lines are commented because there is no need for the address byte anymore.
	 *
	 *
	 *	bytes_to_write[0] = ((adac_addr << 1) | BIT0);//OR with BIT0 to read.
	 *	ack =  i2c_write(1, bytes_to_write, 0);
	 *	if (ack != 0)
	 *	{
	 *		return 1;
	 *	}
	 */

	//The first byte is from the last conversion from the PCF8591, therefore it will be ignored, and we will still read teh next 4 bytes.
	//This is related to the adac locig, and hence should be handled here.
	unsigned char bytes_to_read[5];

	i2c_read(5, bytes_to_read);

	//since we know that the passed array is at least of size 4 bytes
	values[0] = bytes_to_read[1];
	values[1] = bytes_to_read[2];
	values[2] = bytes_to_read[3];
	//values[3] = bytes_to_read[4];//The fourth bit is unnecessary in our exercise, since AIN3 is connect to ground.

	//We won't read the 4th input, sicne it is already grounded.

	return 0;//i2c_write(1, bytes_to_write, 1);//I have to write to stop.
}
// Write a certain value to the DAC.
unsigned char adac_write(unsigned char value)
{
	/*
	 *	The following line is commented because there is no need for the address byte anymore.
	 */
	//const unsigned char adac_addr = 72;//MSB = 0, then address for ADC = 1001000
	const unsigned char adac_control_byte = 64;//from the PCF8591 datasheet


	//unsigned char bytes_to_write[3];
	//bytes_to_write[0] = (adac_addr << 1);	//LSB should be 0 for writing
	//bytes_to_write[1] = adac_control_byte;
	//bytes_to_write[2] = value;

	/*
	 * The following lines are edited versions from the 4 lines above,
	 * because there is no need for an address anymore since the init works
	 */
	unsigned char bytes_to_write [2];
	bytes_to_write[0] = adac_control_byte;
	bytes_to_write[1] = value;

	//write
	return i2c_write(2, bytes_to_write, 1);//the 2 was origanally a 3. but there is no need for the address byte.

}
/*
 *
 * 	Here be dragons.
 *
 */
