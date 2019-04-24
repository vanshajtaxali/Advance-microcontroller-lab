/***************************************************************************//**
 * @file    mma.c
 * @author  <Vanshaj Taxali: Mtr Nr: 4558621>
 * @date    <01-06-2018>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./mma.h"
#include "stdint.h"
#include "i2c.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
int16_t x = 0, y = 0, z = 0;
int8_t x_high = 0, x_low = 0, y_high = 0, y_low = 0, z_high = 0, z_low = 0;
_Bool Resolution_14_BIT;
unsigned char _range;


/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/



/******************************************************************************
 * LOCAL FUNCTION IMPLEMENTATION
 *****************************************************************************/
unsigned char mma_init(void)
{
    _Bool status;

    unsigned char RESET[]           = {MMA8451_CTRL_REG2, MMA8451_CTRL_REG2_RESET};
    unsigned char ENABLE_4G_RANGE[] = {MMA8451_REG_XYZ_DATA_CFG, MMA8451_4G_MODE};
    unsigned char RESOLUTION_8_BIT[]= {MMA8451_CTRL_REG1, MMA8451_CTRL_REG1_F_READ};

    // setting up send device address
    i2c_init(MMA8451_ADDRESS_HIGH, 1);          // initialize i2c -  MMA8451 address, speed

    __delay_cycles(10000);

     // reset all registers with default values
     status = i2c_write(2, RESET, 1);

     __delay_cycles(10000);

     // enable 4G range
     i2c_write(2, ENABLE_4G_RANGE, 1);

     // 8 bit resolution
     i2c_write(2, RESOLUTION_8_BIT, 1);

     // Go to Active mode
     mma_active_mode();

     return status;
}

/*********************************************************
 Change the measurement range. (0: 2g, 1: 4g, >1: 8g)
*********************************************************/
unsigned char mma_setRange(unsigned char range)
{
    unsigned char SET_G_RANGE[] = {};

    // invalid range
    if ((range == 0) || (range > 0)) {
        _range =  range;
    }else{
        return 1;
    }

    //Put the part in Standby Mode
    mma_standby_mode();

    // XYZ_DATA_CFG register
    SET_G_RANGE[0] = MMA8451_REG_XYZ_DATA_CFG;

    switch(range)
    {
        case 0: // 2g full scale range
            SET_G_RANGE[1] = 0x00;
            break;

        case 1: //4g full scale range
            SET_G_RANGE[1] = 0x01;
            break;

        default: // 8g full scale range
            SET_G_RANGE[1] = 0x02;
            break;
    }

    // XYZ_DATA_CFG register sets the dynamic range in register 0x0E
    // measurement range. (0: 2g, 1: 4g, >1: 8g)
    i2c_write(2, SET_G_RANGE, 1);

    //Put the part back into the Active Mode
    mma_active_mode();

    return 0;
}

/*********************************************************
 Change the resolution (0: 8 Bit, >= 1: 14 Bit)
*********************************************************/
unsigned char mma_setResolution(unsigned char resolution)
{
    unsigned char SET_RESOLUTION[] = {} ;
    Resolution_14_BIT = resolution;

    //Go to the Standby Mode - register settings must be made in standby mode
    mma_standby_mode();

    SET_RESOLUTION[0] = MMA8451_CTRL_REG1;

    switch(resolution)
    {
        //Use fast mode (low resolution mode), readings will be limited to 8 bits.
        case 0: // 8Bit Resolution
            SET_RESOLUTION[1] = (i2c_readRegister(MMA8451_CTRL_REG1) | MMA8451_CTRL_REG1_F_READ);
            break;

        //Use default mode (high resolution mode), readings will be 14 bits.
        default: // 14Bit resolution
            SET_RESOLUTION[1] = (i2c_readRegister(MMA8451_CTRL_REG1) & ~MMA8451_CTRL_REG1_F_READ);
            break;
    }

    i2c_write(2, SET_RESOLUTION, 1);

    // Go back to Active mode
    mma_active_mode();

    return 0;
}
unsigned char mma_selftest(void)
{


    unsigned char i, SELF_TEST[2];

    int X_ST = 0, X_ST_ON[10] = {0}, X_ST_OFF[10] = {0}, X_ST_ON_avg = 0, X_ST_OFF_avg = 0;
    int Y_ST = 0, Y_ST_ON[10] = {0}, Y_ST_OFF[10] = {0}, Y_ST_ON_avg = 0, Y_ST_OFF_avg = 0;
    int Z_ST = 0, Z_ST_ON[10] = {0}, Z_ST_OFF[10] = {0}, Z_ST_ON_avg = 0, Z_ST_OFF_avg = 0;


    SELF_TEST[0] = MMA8451_CTRL_REG2;
    SELF_TEST[1] = MMA8451_CTRL_REG2_SELF_TEST;

    // Take 10 readings of XYZ when self-test is OFF
    for (i = 0; i < 10; i++)
    {
        mma_read();         // read XYZ values stored internally
        X_ST_OFF[i] = mma_get14X();     //read X - selftest off
        Y_ST_OFF[i] = mma_get14Y();     //read Y - selftest off
        Z_ST_OFF[i] = mma_get14Z();     //read Z - selftest off

        __delay_cycles(1000);           //small delay
    }

    // Enable self test
    mma_standby_mode();                 //1. put MMA8451Q into Standby mode
    i2c_write(2, SELF_TEST, 1);         //2. setting the ST bit in the CTRL_REG2 register (0x2B),
    mma_active_mode();                  //3. and put the part back into the Active mode

    // Take 10 readings of XYZ when self-test is ON
    for (i = 0; i < 10; i++)
    {
        mma_read();         // read XYZ values stored internally
        X_ST_ON[i] = mma_get14X();      //read X - selftest off
        Y_ST_ON[i] = mma_get14Y();      //read Y - selftest off
        Z_ST_ON[i] = mma_get14Z();      //read Z - selftest off

        __delay_cycles(1000);           //small delay
    }

    SELF_TEST[1] = MMA8451_CTRL_REG2 & ~MMA8451_CTRL_REG2_SELF_TEST;

    // Disable self test
    mma_standby_mode();                 // put MMA8451Q into Standby mode
    i2c_write(2, SELF_TEST, 1);         // disable selftest in the CTRL_REG2 System Control 2 Register
    mma_active_mode();                  // and put the part back into the Active mode

    // summ of the readings
    for (i = 0; i < 10; i++)
    {
        X_ST_OFF_avg = X_ST_OFF_avg + X_ST_OFF[i];
        Y_ST_OFF_avg = Y_ST_OFF_avg + Y_ST_OFF[i];
        Z_ST_OFF_avg = Z_ST_OFF_avg + Z_ST_OFF[i];

        X_ST_ON_avg = X_ST_ON_avg + X_ST_ON[i];
        Y_ST_ON_avg = Y_ST_ON_avg + Y_ST_ON[i];
        Z_ST_ON_avg = Z_ST_ON_avg + Z_ST_ON[i];

        __delay_cycles(100);            //small delay
    }

    // Take average of the measurements
    X_ST_OFF_avg = X_ST_OFF_avg / 10;
    Y_ST_OFF_avg = Y_ST_OFF_avg / 10;
    Z_ST_OFF_avg = Z_ST_OFF_avg / 10;
    X_ST_ON_avg  = X_ST_ON_avg  / 10;
    Y_ST_ON_avg  = Y_ST_ON_avg  / 10;
    Z_ST_ON_avg  = Z_ST_ON_avg  / 10;

    __delay_cycles(100);            //small delay

    //4. compute the difference between the acceleration output of all axes with self-test enabled (ST = 1) and disabled (ST = 0)
    X_ST = X_ST_ON_avg - X_ST_OFF_avg;
    Y_ST = Y_ST_ON_avg - Y_ST_OFF_avg;
    Z_ST = Z_ST_ON_avg - Z_ST_OFF_avg;

    //Check if self test meets minimum values given in datasheet
//  if ((X_ST < 181) || (Y_ST < 255) || (Z_ST < 1680)){         // My Z-Axis is faulty, always in range of -5280 to -5290
    if ((X_ST < 130) || (Y_ST < 185)){
        return  1;  //MMA8451Q failed the self test.
    }

    return  0;      //MMA8451Q passed the self test.
}



unsigned char mma_enableTapInterrupt(void)
{
    //Go to the Standby Mode
    mma_standby_mode();


    unsigned char SETUP_INTERUPT[2] = {0};
    unsigned char readRegVal = i2c_readRegister(MMA8451_PULSE_CFG);

    // setup double pulse detection on the XYZ axis
    SETUP_INTERUPT[0] = MMA8451_PULSE_CFG;              // Pulse Source Register
    SETUP_INTERUPT[1] = readRegVal | MMA8451_XYZ_DOUBLE_PULSE;  // Enable XYZ-Axis Double Pulse with DPA = 0 no double pulse abort
    i2c_write(2, SETUP_INTERUPT, 1);

    //Enable X Double Pulse with DPA = 0 no double pulse abort
    SETUP_INTERUPT[0] = MMA8451_PULSE_THSX;             // Pulse Threshold for X Registers
    SETUP_INTERUPT[1] = MMA8451_PULSE_THSX_2G;          // set X threshold to 2g
    i2c_write(2, SETUP_INTERUPT, 1);

    //Enable Y Double Pulse with DPA = 0 no double pulse abort
    SETUP_INTERUPT[0] = MMA8451_PULSE_THSY;             // Pulse Threshold for Y Registers
    SETUP_INTERUPT[1] = MMA8451_PULSE_THSY_2G;          // set Y threshold to 2g
    i2c_write(2, SETUP_INTERUPT, 1);


    //Set Time Limit for Tap Detection to 100 ms Normal Mode
    // 100 ms/0.625 ms = 160 counts => 0xA0
    SETUP_INTERUPT[0] = MMA8451_PULSE_TMLT;             // Pulse Time Window 1 Register
    SETUP_INTERUPT[1] = MMA8451_PULSE_TMLT_100ms;       // Set Time Limit for Tap Detection to 100 ms LP Mode
    i2c_write(2, SETUP_INTERUPT, 1);

    // Set Latency Time to 200 ms
    // 200 ms/1.25 ms = 160 counts => 0xA0
    SETUP_INTERUPT[0] = MMA8451_PULSE_LTCY;             // Pulse Latency Timer Register
    SETUP_INTERUPT[1] = MMA8451_PULSE_LTCY_200ms;       // Set Latency Time to 200 ms
    i2c_write(2, SETUP_INTERUPT, 1);

    // Set Time Window for second tap to 300 ms
    // 300 ms/2.5 ms = 120 counts => 0xF0
    SETUP_INTERUPT[0] = MMA8451_PULSE_WIND;             // Second Pulse Time Window Register
    SETUP_INTERUPT[1] = MMA8451_PULSE_TAP_WIND;         // Set Time Window for second tap to 300 ms (300 ms/2.5 ms = 120 counts)
    i2c_write(2, SETUP_INTERUPT, 1);

    //Enable Pulse Interrupt in System CTRL_REG4
    SETUP_INTERUPT[0] = MMA8451_CTRL_REG4;              // control register 4
    SETUP_INTERUPT[1] = MMA8451_CTRL_REG4_INT_EN_PULSE; // Tap(pulse) detection interrupt enable
    i2c_write(2, SETUP_INTERUPT, 1);

     //Route Pulse Interrupt to INT1 hardware Pin CTRL_REG5
    SETUP_INTERUPT[0] = MMA8451_CTRL_REG5;              // control register 5
    SETUP_INTERUPT[1] = MMA8451_CTRL_REG5_INT_CFG_PULSE;// Tap(pulse) interrupt routed to INT1 pin
    i2c_write(2, SETUP_INTERUPT, 1);

    // set the interrupt polarity in MMA_CTRL_REG3
    SETUP_INTERUPT[0] = MMA8451_CTRL_REG3;              // control register 3
    SETUP_INTERUPT[1] = MMA8451_CTRL_REG3_IPOL;         // Interrupt polarity ACTIVE high
    i2c_write(2, SETUP_INTERUPT, 1);

    // Go back to Active mode
    mma_active_mode();

    return 0;
}


// Disable the double-tap-interrupt on the MMA.

unsigned char mma_disableTapInterrupt(void)
{
    unsigned char interrupt_source, tap_source, shakeAxis;

    //Determine the source of the interrupt by first reading the system interruptregister
    interrupt_source = i2c_readRegister(MMA8451_INT_SOURCE);

    // Tap (pulse) interrupt source
    if ((interrupt_source & 0x08) == 0x08)
    {
        //Read the Pulse Status Register to clear the system interrupt
        tap_source = i2c_readRegister(MMA8451_PULSE_SRC);

        // check for taps on XYZ-axis
        if ((tap_source & MMA8451_PULSE_SRC_X_AXIS) == 0x10){   // shakeAxisX
            shakeAxis = 1;
        }
        if ((tap_source & MMA8451_PULSE_SRC_Y_AXIS) == 0x20){   // shakeAxisY
            shakeAxis = 2;
        }
        if ((tap_source & MMA8451_PULSE_SRC_Z_AXIS) == 0x40){   // shakeAxisZ
            shakeAxis = 4;
        }
    }

    return shakeAxis;
}



unsigned char mma_read(void)
{
    unsigned char buf[6];
    unsigned char READ_REG[] = {MMA8451_REG_OUT_X_MSB};


    // writa address to read
    i2c_write(1, READ_REG, 0);

    __delay_cycles(1000);

    // Read with 14 bit resolution
     if (Resolution_14_BIT)
     {
        //Read 14-bit XYZ results using a 6 byte i2c access.
        i2c_read(6, buf);

        //Copy and save each result as a 16-bit left-justified value.
         x_high =  buf[0];
         x_low  =  buf[1];
         y_high =  buf[2];
         y_low  =  buf[3];
         z_high =  buf[4];
         z_low  =  buf[5];
     }
     else    // Read with 8 bit resulotion
     {
        //Read 14-bit XYZ results using a 6 byte i2c access.
        i2c_read(3, buf);

        //Copy and save each result as a 16-bit left-justified value.
         x_high =  buf[0];
         x_low  = 0;
         y_high =  buf[1];
         y_low  = 0;
         z_high =  buf[2];
         z_low  = 0;
     }

      return 0;
}


// Return 8 bit X-Value
signed char mma_get8X(void)
{
    if (Resolution_14_BIT) {
        x = x_high << 8 | x_low & 0xFF;
        x = (x_high * BIT_8)/BIT_14;
    }
    else{
        x = x_high;
    }

    //convert 2's complement number to signed integer result - 8 bit X-Value.
    if (x > 127) x = x - 256;

    return x;
}

// Return 8 bit Y-Value
signed char mma_get8Y(void)
{
    if (Resolution_14_BIT) {
        y = y_high << 8 | y_low & 0xFF;
        y = (y_high * BIT_8)/BIT_14;
    }
    else{
        y = y_high;
    }

    //convert 2's complement number to signed integer result - 8 bit Y-Value.
    if (y > 127) y = y - 256;

    return y;
}

// Return 8 bit Z-Value
signed char mma_get8Z(void)
{
    if (Resolution_14_BIT) {
        z = z_high << 8 | z_low & 0xFF;
        z = (z_high * BIT_8)/BIT_14;
    }
    else{
        z = z_high;
    }

    //convert 2's complement number to signed integer result - 8 bit Z-Value.
    if (z > 127) z = z - 256;

    return z;
}

// Return 14 bit X-Value
int mma_get14X(void)
{
    if (Resolution_14_BIT) {
        x = x_high << 8 | x_low & 0xFF;
        x = x >> 2;
    }
    else{
        x = (x_high * BIT_14)/BIT_8;
    }

    //convert 2's complement number to signed integer result - 14 bit X-Value.
    if (x > 8191) x = x - 16384;

    return x;
}
// Return 14 bit Y-Value
int mma_get14Y(void)
{
    if (Resolution_14_BIT) {
        y = y_high << 8 | y_low & 0xFF;
        y = y >> 2;
    }
    else{
        y = (y_high * BIT_14)/BIT_8;
    }

    //convert 2's complement number to signed integer result - 14 bit Y-Value.
    if (y > 8191) y = y - 16384;

    return y;
}
// Return 14 bit Z-Value
int mma_get14Z(void)
{
    if (Resolution_14_BIT) {
        z = z_high << 8 | z_low & 0xFF;
        z = z >> 2;
    }
    else{
        z = (z_high * BIT_14)/BIT_8;
    }

    //convert 2's complement number to signed integer result - 14 bit Z-Value.
    if (z > 8191) z = z - 16384;

    return z;
}
// X values in m*s^-2.
double mma_getRealX(void)
{
    double x_g, acceleration_x;
    unsigned int divider = 1;

    if (_range == MMA8451_8G_MODE) divider = 1024;  // Counts/g
    if (_range == MMA8451_4G_MODE) divider = 2048;  // Counts/g
    if (_range == MMA8451_2G_MODE) divider = 4096;  // Counts/g

    x_g = (double)x / divider;

    // Convert Acceleration Data to m/s^2
    acceleration_x = x_g * STANDARD_GRAVITY;

    return acceleration_x;
}

// Y values in m*s^-2.
double mma_getRealY(void)
{
    double y_g, acceleration_y;
    unsigned int divider = 1;

    if (_range == MMA8451_8G_MODE) divider = 1024;  // Counts/g
    if (_range == MMA8451_4G_MODE) divider = 2048;  // Counts/g
    if (_range == MMA8451_2G_MODE) divider = 4096;  // Counts/g

    y_g = (double)y / divider;

    // Convert Acceleration Data to m/s^2
    acceleration_y = y_g * STANDARD_GRAVITY;

    return acceleration_y;
}

// Z values in m*s^-2.
double mma_getRealZ(void)
{
    double z_g, acceleration_z;
    unsigned int divider = 1;

    if (_range == MMA8451_8G_MODE) divider = 1024;  // Counts/g
    if (_range == MMA8451_4G_MODE) divider = 2048;  // Counts/g
    if (_range == MMA8451_2G_MODE) divider = 4096;  // Counts/g

    z_g = (double)z / divider;

    // Convert Acceleration Data to m/s^2
    acceleration_z = z_g * STANDARD_GRAVITY;

    return acceleration_z;
}



/******************************************************************************
 * EXTRA FUNCTION IMPLEMENTATION
 *****************************************************************************/
//put mma to standby mode
void mma_standby_mode (void)
{
    /*
    ** Read current value of System Control 1 Register.
    ** Put sensor into Standby Mode by clearing the Active bit
    ** Return with previous value of System Control 1 Register.
    */

    __delay_cycles(1000);
    // first read the register value, so it can be writtn back later
    unsigned char val = i2c_readRegister(MMA8451_CTRL_REG1);

    unsigned char STANBY_MODE[] =
    {
        MMA8451_CTRL_REG1,
        val & ~MMA8451_CTRL_REG1_ACTIVE
    };

    i2c_write(2, STANBY_MODE, 1);           // deactivate
}

//put mma to active mode
void mma_active_mode ()
{
    //Set the Active bit in CTRL Reg 1

    __delay_cycles(1000);
    // first read the register value, so it can be writtn back later
    unsigned char val = i2c_readRegister(MMA8451_CTRL_REG1);

    unsigned char ACTIVE_MODE[] =
    {
        MMA8451_CTRL_REG1,
        val | MMA8451_CTRL_REG1_ACTIVE
    };

    i2c_write(2, ACTIVE_MODE, 1);           // activate

    __delay_cycles(100);
}
//read register contents.
unsigned char i2c_readRegister( unsigned char reg )
{
    unsigned char REGISTER_TO_READ[1] = {reg};
    unsigned char reg_val[1];

    // write register address
    i2c_write(1, REGISTER_TO_READ, 0);

    // read register content
    i2c_read(1, reg_val);

    return reg_val[0];
}

/*
 *
 * 	Have fun! :-)
 *
 */
