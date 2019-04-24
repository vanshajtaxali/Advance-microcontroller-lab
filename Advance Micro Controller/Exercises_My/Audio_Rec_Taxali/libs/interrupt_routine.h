/***************************************************************************//**
 * @file    interrupt_routine.h
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <14-06-18>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_INTERRUPT_ROUTINE_H_
#define LIBS_INTERRUPT_ROUTINE_H_

//#include "i2c.h"
//#include "spi.h"

extern volatile unsigned char* i2c_dataBuffer;
extern volatile unsigned char i2c_dataLength;
extern volatile unsigned char i2c_nackReceived;
extern volatile unsigned char* spi_rxdataBuffer;
extern volatile unsigned char* spi_txdataBuffer;
extern volatile unsigned char spi_rxdataLength;
extern volatile unsigned char spi_txdataLength;

extern unsigned char IAM_i2c;
extern unsigned char IAM_spi;
extern unsigned char IAM_spiTX;
extern unsigned char IAM_spiRX;


#endif /* LIBS_INTERRUPT_ROUTINE_H_ */
