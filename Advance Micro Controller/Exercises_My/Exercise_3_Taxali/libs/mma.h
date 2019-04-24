/***************************************************************************//**
 * @file    mma.h
 * @author  <Vanshaj Taxali>
 * @date    <01/06/2018>
 * @email:  <vanshajtaxali94@gmail.com>
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 *
 * @ note   The X/Y-direction printed on the board are wrong:
//          X is the real Y and Y is the real X. Sorry for the confusion. :-(
 ******************************************************************************/

#ifndef EXERCISE_3_LIBS_MMA_H_
#define EXERCISE_3_LIBS_MMA_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "./i2c.h"

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/
#define MMA8451_ADDRESS_LOW             0x1C // address pin low (GND)
#define MMA8451_ADDRESS_HIGH            0x1D // address pin high (VCC)
#define MMA8451_DEVICE_ID               0x1A //
//Double tap interrupt
#define MMA8451_PULSE_CFG               0x21 //Pulse Source Register
#define MMA8451_XYZ_DOUBLE_PULSE        0x2A //Enable XYZ-Axis Double Pulse with DPA = 0 no double pulse abort
#define MMA8451_PULSE_THSX              0x23 // Pulse Threshold for X Registers
#define MMA8451_PULSE_THSY              0x24 // Pulse Threshold for Y Registers
#define MMA8451_PULSE_THSZ              0x25 // Pulse Threshold for Z Registers
#define MMA8451_PULSE_THSX_2G           0x20 // set X threshold to 2g (2 g/0.063g = 32 counts)
#define MMA8451_PULSE_THSY_2G           0x20 // set Y threshold to 2g (2 g/0.063g = 32 counts)
#define MMA8451_PULSE_THSZ_2G           0x20 // set Z threshold to 2g (2 g/0.063g = 32 counts)
#define MMA8451_PULSE_TMLT              0x26 // Pulse Time Window 1 Register
#define MMA8451_PULSE_TMLT_100ms        0xA0 // Set Time Limit for Tap Detection to 100 ms LP Mode (100 ms/0.625 ms = 160 counts)
#define MMA8451_PULSE_LTCY              0x27 // Pulse Latency Timer Register
#define MMA8451_PULSE_LTCY_200ms        0xA0 // Set Latency Time to 200 ms (200 ms/1.25 ms = 160 counts)
#define MMA8451_PULSE_WIND              0x28 // Second Pulse Time Window Register
#define MMA8451_PULSE_TAP_WIND          0xF0 // Set Time Window for second tap to 300 ms (300 ms/1.25 ms = 240 counts)
#define MMA8451_INT_SOURCE              0x0C // Interrupt Source Status Register
#define MMA8451_PULSE_SRC               0x22 // Pulse Source Register
#define MMA8451_PULSE_SRC_X_AXIS        0x10 // X-Axis event
#define MMA8451_PULSE_SRC_Y_AXIS        0x20 // Y-Axis event
#define MMA8451_PULSE_SRC_Z_AXIS        0x40 // Z-Axis event
//Out Registers
#define MMA8451_REG_OUT_X_MSB           0x01
#define MMA8451_REG_OUT_X_LSB           0x02
#define MMA8451_REG_OUT_Y_MSB           0x03
#define MMA8451_REG_OUT_Y_LSB           0x04
#define MMA8451_REG_OUT_Z_MSB           0x05
#define MMA8451_REG_OUT_Z_LSB           0x06
//Sensitivity modes
#define MMA8451_REG_XYZ_DATA_CFG        0x0E
#define MMA8451_2G_MODE                 0x00 //Set Sensitivity to 2g
#define MMA8451_4G_MODE                 0x01 //Set Sensitivity to 4g
#define MMA8451_8G_MODE                 0x02 //Set Sensitivity to 8g
//Control Register 1
#define MMA8451_CTRL_REG1               0x2A
#define MMA8451_CTRL_REG1_STANDBY       0x00
#define MMA8451_CTRL_REG1_ACTIVE        0x01
#define MMA8451_CTRL_REG1_F_READ        0x02
#define MMA8451_CTRL_REG1_DR0           0x00 //800 Hz, Standby Mode
//Control Register 2
#define MMA8451_CTRL_REG2               0x2B //CTRL_REG2 System Control 2 Register
#define MMA8451_CTRL_REG2_RESET         0x40 //reset: 01000000
#define MMA8451_CTRL_REG2_SELF_TEST     0x80 //self test
//Control Register 3
#define MMA8451_CTRL_REG3               0x2C //CTRL_REG3 Interrupt Control Register
#define MMA8451_CTRL_REG3_PP_OD         0x01 //Push-Pull/Open Drain selection on interrupt pad
#define MMA8451_CTRL_REG3_IPOL          0x02 //Interrupt polarity ACTIVE high
//Control Register 4
#define MMA8451_CTRL_REG4               0x2D //CTRL_REG4 Register (Read/Write)
#define MMA8451_CTRL_REG4_INT_EN_PULSE  0x08 // Tap(pulse) detection interrupt
//Control Register 5
#define MMA8451_CTRL_REG5               0x2E //CTRL_REG5 Register (Read/Write)
#define MMA8451_CTRL_REG5_INT_CFG_PULSE 0x08 // Tap(pulse) interrupt routed to INT1 pin
// Constants
#define STANDARD_GRAVITY                (9.80665)   // Gravitational accelration
#define BIT_14                          16383       // 2^14
#define BIT_8                           255         // 2^8

/******************************************************************************
 * VARIABLES
 *****************************************************************************/



/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

// All configuration functions return 0 if everything went fine
// and anything but 0 if not (they are the ones with a unsigned char return type).

// Initialize the MMA with 8 bit resolution and 4G measurement range (1 pt.)
unsigned char mma_init(void);

// Change the measurement range. (0: 2g, 1: 4g, >1: 8g) (0.5 pt.)
unsigned char mma_setRange(unsigned char range);
// Change the resolution (0: 8 Bit, >= 1: 14 Bit) (0.5 pt.)
unsigned char mma_setResolution(unsigned char resolution);

// Run a self-test on the MMA, verifying that all three axis and all three
// measurement ranges are working. (1 pt.)
/* HINT:
 * The idea of the self test is that you measure the current acceleration values,
 * then enable the on-chip self-test and then read the values again.
 * The values without selftest enabled and those with selftest enabled
 * should now feature a predefined difference (see the datasheet).
 */
unsigned char mma_selftest(void);


// Set up the double tap interrupt on the MMA (do not set up the interrupt on
// the MSP in this function!). This means that the MMA should change the INT1-
// pin whenever a double tap is detected. You may freely choose the axis on
// which the tap has to be received. (You should put a comment in your code,
// which axis you chose, though). (1 pt.)
/* HINT:
 * As the datasheet for the MMA is a bit stingy when it comes to the double
 * tap stuff, so here's (roughly) what you should do:
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
 */
unsigned char mma_enableTapInterrupt(void);
// Disable the double-tap-interrupt on the MMA. (0.5 pt.)
unsigned char mma_disableTapInterrupt(void);


// Read the values of all three axis from the chip and store the values
// internally. Take the requested resolution into account. (1 pt.)
unsigned char mma_read(void);


/* Get Functions (1 pt. total): */

// Return the appropriate 8 bit values
// If the resolution during mma_read was 14 bit, translate the data to 8 bit
signed char mma_get8X(void);
signed char mma_get8Y(void);
signed char mma_get8Z(void);

// Return the appropriate 14 bit values
// If the resolution during mma_read was 8 bit, translate the data to 14 bit
int mma_get14X(void);
int mma_get14Y(void);
int mma_get14Z(void);

// Return the appropriate values in m*s^-2.
double mma_getRealX(void);
double mma_getRealX(void);
double mma_getRealX(void);

void mma_standby_mode (void);
void mma_active_mode (void);
unsigned char i2c_readRegister( unsigned char reg );
#endif /* EXERCISE_3_LIBS_MMA_H_ */
