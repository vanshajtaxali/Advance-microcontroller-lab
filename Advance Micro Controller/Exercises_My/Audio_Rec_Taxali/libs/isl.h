/***************************************************************************//**
 * @file    isl.h
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <1-07-2018>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_ISL_H_
#define LIBS_ISL_H_

#define ISL95811_ADDRESS	0x28	//device address
#define INITIAL_VALUE   	0x80	//initial value
#define REG_IVR				0x00	//IVR also writes to WR
#define	REG_WR				0x80	//wiper register
#define	ACR					0x08	//


unsigned char isl_init(void);
// Set the wiper to <wiperValue>.
unsigned char isl_set(unsigned char wiperValue);

#endif /* LIBS_ISL_H_ */
