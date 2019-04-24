/*
 * mma.c
 *
 *  Created on: 05.06.2015
 *      Author: <Hassan Gamaleldin>
 */

#include "./mma.h"
#include "i2c.h"

/*
 *
 * 	Here be dragons.
 *
 */
//Address
#define MYADDR					(0x1D)         /* This is the slave address */
//Registers -- Please check the datasheet with the exact names below for more details
#define OUT_X_MSB				(0x01)
#define OUT_X_LSB				(0x02)
#define OUT_Y_MSB				(0x03)
#define OUT_Y_LSB				(0x04)
#define OUT_Z_MSB				(0x05)
#define OUT_Z_LSB				(0x06)
#define CTRL_REG1				(0x2A)			/* System Control 1 Register */
#define CTRL_REG2				(0x2B)			/* System Control 2 Register */
#define CTRL_REG3				(0x2C)			/* System Control 3 Register */
#define CTRL_REG4				(0x2D)         	/* System Control 4 Register */
#define CTRL_REG5				(0x2E)			/* System Control 5 Register */
#define XYZ_DATA_CFG			(0x0E)			/* Data Configuration Register */
#define PULSE_CFG				(0x21)			/* For double tap  */
#define PULSE_THSX				(0x23)			/* For double tap */
#define PULSE_THSY				(0x24)			/* For double tap */
#define PULSE_THSZ				(0x25)			/* For double tap */
#define PULSE_TMLT				(0x26)			/* For double tap */
#define PULSE_LTCY				(0x27)			/* For double tap */
#define PULSE_WIND				(0x28)			/* For double tap */

//Registers' Vlaues
//CTRL_REG1
#define LNOISE					(0x04)         /*  */
#define F_READ					(0x02)         /* Fast mode, 8-bit if active */
#define ACTIVE					(0x01)         /* To activate or standby the mma */

//CTRL_REG2
#define ST						(0x80)         /* To enable self-test */

//CTRL_REG3
#define IPOL					(0x02)         /* interrupt polarity */

//CTRL_REG4
#define INT_EN_PULSE			(0x08)         /* to enable interrupt */

//CTRL_REG5
#define INT_CFG_PULSE			(0x08)         /* to route interrupt on INT1 */

//PULSE_CFG
#define ZDPEFE					(0x20)

//XYZ_DATA_CFG
/*
 * Clear xyzCFGRegVal before oring to the following values
 * No need for range 2 as it is the default value
 * */
#define range8					(0x02)
#define range4					(0x01)

unsigned char ctrlRegVal_1 = 0x00;
unsigned char ctrlRegVal_2 = 0x00;
unsigned char ctrlRegVal_3 = 0x00;
unsigned char ctrlRegVal_4 = 0x00;
unsigned char ctrlRegVal_5 = 0x00;
unsigned char xyzCFGRegVal = 0x00;

#define MAX_8_BIT 255
#define MAX_14_BIT 16383

unsigned char myResolution;
unsigned char myRange;

const float g = 9.80665;

unsigned char xVal8;
unsigned char yVal8;
unsigned char zVal8;
int xVal14;
int yVal14;
int zVal14;

// Initialize the MMA with 8 bit resolution and 4G measurement range
unsigned char mma_init(void) {
	unsigned char myAddr = MYADDR;
	i2c_init(myAddr, 1);

	unsigned char status = 0x00;
	status |= mma_standby();

	status |= mma_setResolution(0);

	status |= mma_setRange(1);

	status |= mma_activate();

	//call 2 functions here
	return status;
}

// Change the measurement range. (0: 2g, 1: 4g, >1: 8g)
unsigned char mma_setRange(unsigned char range) {

	myRange = range;
	xyzCFGRegVal = 0x00; //this has to be clread
	if (range == 1) {
		xyzCFGRegVal |= range4;
	} else if (range > 1) {
		xyzCFGRegVal |= range8;
	}

	unsigned char bytes_to_write[2];
	bytes_to_write[0] = XYZ_DATA_CFG;
	bytes_to_write[1] = xyzCFGRegVal;

	/*
	 * the zero condition is still satisfied as the paremeter's range is 0-255,
	 * and the above if statement covers 1 - 255 of that range, leaving 0 to the
	 * defdault value 0x00
	 */
	unsigned char status = 0x00;

	status |= i2c_write(2, bytes_to_write, 1);
	return status;
}
// Change the resolution (0: 8 Bit, >= 1: 14 Bit)
unsigned char mma_setResolution(unsigned char resolution) {

	if (resolution == 0) {
		ctrlRegVal_1 |= F_READ;
	} else {
		ctrlRegVal_1 &= (~F_READ);
	}
	myResolution = resolution; //to keep the resolution for other functions.

	unsigned char bytes_to_write[2];
	bytes_to_write[0] = CTRL_REG1;
	bytes_to_write[1] = ctrlRegVal_1;

	unsigned char status = 0x00;

	status |= i2c_write(2, bytes_to_write, 1);

	return status;
}

// Run a self-test on the MMA, verifying that all three axis and all three
// measurement ranges are working.
unsigned char mma_selftest(void) {

	/*
	 * Also, about the selftest:
	 * The idea is that you measure the current values, then enable the on-chip
	 * self-test and then read the values again. The values without the selftest
	 * enabled and those with the selftest enabled should now be a certain, pre-
	 * defined value (see the datasheet) apart from each other.
	 */
	int x, y, z;
	int xST, yST, zST;
	int xDiff, yDiff, zDiff;

	unsigned char status = 0x00;
	unsigned char bytes_to_write[2];
	int i;
	for(i = 0; i < 10; i++) {
		status |= mma_activate();

		status |= mma_read();

		if (myResolution == 0) {
			x = mma_get8X();
			y = mma_get8Y();
			z = mma_get8Z();
		} else {
			x = mma_get14X();
			y = mma_get14Y();
			z = mma_get14Z();
		}

		status |= mma_standby();

//--------------------------------------
		bytes_to_write[0] = CTRL_REG2;
		bytes_to_write[1] = ST;//enable self-test

		status |= i2c_write(2, bytes_to_write, 1);

		status |= mma_activate();

		status |= mma_read();

		if (myResolution == 0) {
			xST = mma_get8X();
			yST = mma_get8Y();
			zST = mma_get8Z();
		} else {
			xST = mma_get14X();
			yST = mma_get14Y();
			zST = mma_get14Z();
		}

		status |= mma_standby();

		//--------------------------------------
		bytes_to_write[0] = CTRL_REG2;
		bytes_to_write[1] &= (~ST);//disable self-test

		status |= i2c_write(2, bytes_to_write, 1);

		status |= mma_activate();

		xDiff = x - xST;
		yDiff = y - yST;
		zDiff = z - zST;
	}

	xDiff = xDiff/10;
	yDiff = yDiff/10;
	zDiff = zDiff/10;

	if (xDiff < 120) { //threshold for x
		status |= 0x01;
	}
	if (yDiff < 180) { //threshold for y
		status |= 0x01;
	}

	return status;
}

// Set up the double tap interrupt on the MMA (do not set up the interrupt on
// the MSP in this function!). This means that the MMA should change the INT1-
// pin whenever a double tap is detected. You may freely choose the axis on
// which the tap has to be received. (You should put a comment in your code,
// which axis you chose, though).
unsigned char mma_enableTapInterrupt(void) {

	/*
	 *  1) Go to standby (as you can only change the registers when in standby)
	 *  2) Write MMA_PULSE_CFG  to enable the z-axis for double tap
	 *  3) Write MMA_PULSE_THSZ to set the tap threshold (e.g. to 2g)
	 *  4) Write MMA_PULSE_TMLT to set the pulse time limit (e.g. to 100 ms)
	 *  5) Write MMA_PULSE_LTCY to set the pulse latency timer (e.g. to 200 ms)
	 *  6) Write MMA_PULSE_WIND to set the time window for the second tap
	 *  7) Write MMA_CTRL_REG4  to set the pulse interrupt
	 *  8) Write MMA_CTRL_REG5  to activate the interrupt on INT1
	 *  9) Write MMA_CTRL_REG3  to set the interrupt polarity
	 * 10) Return to active mode
	 * */
	unsigned char status = 0x00;

	status |= mma_standby();

	unsigned char bytes_to_write[2];

//Write MMA_PULSE_CFG  to enable the z-axis for double tap
	bytes_to_write[0] = PULSE_CFG;
	bytes_to_write[1] = 0x2A; //ZDPEFE

	status |= i2c_write(2, bytes_to_write, 1);

//Write MMA_PULSE_THSZ to set the tap threshold (e.g. to 2g)
	bytes_to_write[0] = PULSE_THSZ;
	bytes_to_write[1] = 0x20;
	/*
	 * There is a typo either in the datasheet, or the Application note
	 * Datasheet says step is 0.63g/LSB, AN says 0.063g/LSB
	 */
	status |= i2c_write(2, bytes_to_write, 1);

//Write MMA_PULSE_TMLT to set the pulse time limit (e.g. to 100 ms)
	bytes_to_write[0] = PULSE_TMLT;
	bytes_to_write[1] = 0xA0; //100ms/0.625ms

	status |= i2c_write(2, bytes_to_write, 1);

//Write MMA_PULSE_LTCY to set the pulse latency timer (e.g. to 200 ms)
	bytes_to_write[0] = PULSE_LTCY;
	bytes_to_write[1] = 0xA0; //200ms/1.25ms

	status |= i2c_write(2, bytes_to_write, 1);

//Write MMA_PULSE_WIND to set the time window for the second tap
	bytes_to_write[0] = PULSE_WIND;
	bytes_to_write[1] = 0xFF; //highest possible window, which is 318.72

	status |= i2c_write(2, bytes_to_write, 1);

//Write MMA_CTRL_REG4  to set the pulse interrupt
	bytes_to_write[0] = CTRL_REG4;
	bytes_to_write[1] = INT_EN_PULSE;

	status |= i2c_write(2, bytes_to_write, 1);

//Write MMA_CTRL_REG5  to activate the interrupt on INT1
	bytes_to_write[0] = CTRL_REG5;
	bytes_to_write[1] = INT_CFG_PULSE;

	status |= i2c_write(2, bytes_to_write, 1);

//Write MMA_CTRL_REG3  to set the interrupt polarity
	bytes_to_write[0] = CTRL_REG3;
	bytes_to_write[1] &= (~IPOL); //active low

	status |= i2c_write(2, bytes_to_write, 1);

	status |= mma_activate();

	return status;
}
// Disable the double-tap-interrupt on the MMA.
unsigned char mma_disableTapInterrupt(void) {

	unsigned char status = 0x00;

	status |= mma_standby();

	unsigned char bytes_to_write[2];
	bytes_to_write[0] = PULSE_CFG;
	bytes_to_write[1] = 0x00; //Revert to the default value

	status |= i2c_write(2, bytes_to_write, 1);

	bytes_to_write[0] = CTRL_REG4;
	bytes_to_write[1] = 0x00; //Revert to the default value

	status |= i2c_write(2, bytes_to_write, 1);

	bytes_to_write[0] = CTRL_REG5;
	bytes_to_write[1] = 0x00; //Revert to the default value

	status |= mma_activate();
	return status;
}

// Read the values of all three axis from the chip and store the values
// internally. Take the requested resolution into account.
unsigned char mma_read(void) {

	unsigned char status = 0x00;
	unsigned char bytes_to_write[1];
	unsigned char three_bytes_to_read[3];
	unsigned char six_bytes_to_read[6];

	bytes_to_write[0] = OUT_X_MSB;
	status |= i2c_write(1, bytes_to_write, 1);

	if (myResolution == 0) {
		i2c_read(3, three_bytes_to_read);

		xVal8 = three_bytes_to_read[0];

		yVal8 = three_bytes_to_read[1];

		zVal8 = three_bytes_to_read[2];
	} else {
		i2c_read(6, six_bytes_to_read);

		xVal14 = six_bytes_to_read[0];
		xVal14 <<= 8;

		xVal14 |= six_bytes_to_read[1];
		xVal14 >>= 2;

		yVal14 = six_bytes_to_read[2];
		yVal14 <<= 8;

		yVal14 |= six_bytes_to_read[3];
		yVal14 >>= 2;

		zVal14 = six_bytes_to_read[4];
		zVal14 <<= 8;

		zVal14 |= six_bytes_to_read[5];
		zVal14 >>= 2;
	}
	return status;
}

// Important notice: The X/Y-direction printed on the board are wrong:
// X is the real Y and Y is the real X. Sorry for the confusion. :-(

// Return the appropriate 8 bit values
// If the resolution during mma_read was 14 bit, translate the data to 8 bit
signed char mma_get8X(void) {
	if (myResolution > 0) {
		return ((xVal14 * MAX_8_BIT) / MAX_14_BIT);
	} else {
		return xVal8;
	}
}
signed char mma_get8Y(void) {
	if (myResolution > 0) {
		return ((yVal14 * MAX_8_BIT) / MAX_14_BIT);
	} else {
		return yVal8;
	}
}
signed char mma_get8Z(void) {
	if (myResolution > 0) {
		return ((zVal14 * MAX_8_BIT) / MAX_14_BIT);
	} else {
		return zVal8;
	}
}

// Return the appropriate 14 bit values
// If the resolution during mma_read was 8 bit, translate the data to 14 bit
int mma_get14X(void) {
	if (myResolution == 0) {
		return ((xVal8 * MAX_14_BIT) / MAX_8_BIT);
	} else {
		return xVal14;
	}
}
int mma_get14Y(void) {
	if (myResolution == 0) {
		return ((yVal8 * MAX_14_BIT) / MAX_8_BIT);
	} else {
		return yVal14;
	}
}
int mma_get14Z(void) {
	if (myResolution == 0) {
		return ((zVal8 * MAX_14_BIT) / MAX_8_BIT);
	} else {
		return zVal14;
	}
}

// Change the resolution (0: 8 Bit, >= 1: 14 Bit)
// Return the appropriate values in m*s^-2.
double mma_getRealX(void) {

	int divider = mma_getDivider();

	float x;
	if (myResolution == 0) {
		x = (float) xVal8;
	} else {
		x = (float) xVal14;
	}

	float x_g = x / divider;

	return (x_g * g);

}
double mma_getRealY(void) {
	int divider = mma_getDivider();

	float y;
	if (myResolution == 0) {
		y = (float) yVal8;
	} else {
		y = (float) yVal14;
	}

	float y_g = y / divider;

	return (y_g * g);
}
double mma_getRealZ(void) {
	int divider = mma_getDivider();

	float z;
	if (myResolution == 0) {
		z = (float) zVal8;
	} else {
		z = (float) zVal14;
	}

	float z_g = z / divider;

	return (z_g * g);

}
// Change the measurement range. (0: 2g, 1: 4g, >1: 8g)
int mma_getDivider(void) {
	if (myRange == 1) {
		return 2048;
	} else if (myRange > 1) {
		return 1024;
	} else {
		return 4096;
	}
}

signed char mma_activate(void) {

	ctrlRegVal_1 |= ACTIVE;

	unsigned char bytes_to_write[2];
	bytes_to_write[0] = CTRL_REG1;
	bytes_to_write[1] = ctrlRegVal_1;

	return i2c_write(2, bytes_to_write, 1);
}

signed char mma_standby(void) {

	ctrlRegVal_1 &= (~ACTIVE);

	unsigned char bytes_to_write[2];
	bytes_to_write[0] = CTRL_REG1;
	bytes_to_write[1] = ctrlRegVal_1;

	return i2c_write(2, bytes_to_write, 1);
}

/* So, the datasheet for the MMA is a bit stingy when it comes to the double
 * tap stuff, so here's (rougly) what you should do:
 *
 *  1) Go to standby (as you can only change the registers when in standby)
 *  2) Write MMA_PULSE_CFG  to enable the z-axis for double tap
 *  3) Write MMA_PULSE_THSZ to set the tap threshold (e.g. to 2g)
 *  4) Write MMA_PULSE_TMLT to set the pulse time limit (e.g. to 100 ms)
 *  5) Write MMA_PULSE_LTCY to set the pulse latency timer (e.g. to 200 ms)
 *  6) Write MMA_PULSE_WIND to set the time window for the second tap
 *  7) Write MMA_CTRL_REG4  to set the pulse interrupt
 *  8) Write MMA_CTRL_REG5  to activate the interrupt on INT1
 *  9) Write MMA_CTRL_REG3  to set the interrupt polarity
 * 10) Return to active mode
 *
 * Also, about the selftest:
 * The idea is that you measure the current values, then enable the on-chip
 * self-test and then read the values again. The values without the selftest
 * enabled and those with the selftest enabled should now be a certain, pre-
 * defined value (see the datasheet) apart from each other.
 *
 */
