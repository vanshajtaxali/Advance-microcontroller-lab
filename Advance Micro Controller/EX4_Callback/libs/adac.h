/***************************************************************************//**
 * @file    adac.h
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <14-06-2018>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef EXERCISE_3_LIBS_ADAC_H_
#define EXERCISE_3_LIBS_ADAC_H_

/* i2c control bytes*/
#define ADC0			0x00	// ADC channel 0
#define ADC1			0x01	// ADC channel 1
#define ADC2			0x02	// ADC channel 2
#define ADC3			0x03	// ADC channel 3
#define AUTO_INCREMENT	0x04	// Auto increment
#define ENABLE_DAC		0x40	// enable analog output
#define PCF8591_addr 	0x48	// i2c address

// All functions return 0 if everything went fine
// and anything but 0 if not.

// Initialize the ADC / DAC
unsigned char adac_init(void);
// Read all ADC-values and write it into the passed values-array.
// (Important: always pass an array of size four (at least).)
unsigned char adac_read(unsigned char * values);
// Write a certain value to the DAC.
unsigned char adac_write(unsigned char value);

#endif /* EXERCISE_3_LIBS_ADAC_H_ */
