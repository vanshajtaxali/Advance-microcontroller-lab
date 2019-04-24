/***************************************************************************//**
 * @file   isl.c
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <1-07-2018>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/
#include "i2c.h"
#include "isl.h"

//initialise the isl for i2c adressing
unsigned char isl_init(void)
{
	i2c_init(ISL95811_ADDRESS, 1);

	return 0;
}
// Set the wiper to <wiperValue>.
unsigned char isl_set(unsigned char wiperValue)
{
	unsigned char WIPER_VALUE[2] = {0};

	WIPER_VALUE[0] = REG_IVR;
	WIPER_VALUE[1] = wiperValue;

	i2c_write(2, WIPER_VALUE, 1);

	return 0;
}


