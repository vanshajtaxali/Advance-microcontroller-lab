/*
 * i2cm.c
 *
 *  Created on: 21.05.2015
 *      Author: Hassan Gamaleldin
 */

#include "./i2cm.h"

/*
 *
 * 	Here be dragons.
 *
 */

unsigned char myAddr;
unsigned char mySpeed;

// Initialize the I2C state machine. Speed can be either 1
// (normal mode, 100 kbit/s) or 4 (Fast Mode, 400 kbit/s).
// You may stick to 100 kbit/s no matter what <speed> says, for now.
// <addr> is the 7-bit address of the slave (MSB shall always be 0, i.e. "right alignment")
void i2c_init (unsigned char addr, unsigned char speed)
{
	//can't find a use fot init for now, but probably when the considering more speed options, then can it be more useful
	myAddr = addr;
	mySpeed = speed;
}
// Write a sequence of <length> characters from the pointer <txData>.
// Return 0 if acknowledged, 1 if not. Also stop transmitting further bytes upon a missing acknowledge.
// Only send a stop condition if <stop> is not 0
unsigned char i2c_write(unsigned char length, unsigned char * txData, unsigned char stop)
{
	int char_num = length;

	unsigned char curr_bit;
	int i,j;
	int ack = 0;

	i2c_start();

	for(i = 0; i < char_num; i++)
	{
		P2OUT |= BIT5; //we have to be sure scl is pulled low before we start sending a byte
		i2c_delay();
		for(j = 7; j >= 0; j--)
		{
			curr_bit = ((txData[i]&(BIT0<<j))>>j);//first we BIT0 left by j to focus on the nth bit (using the and operation), and then shift this value back to the LSB
			if (curr_bit == 0)
			{
				P2OUT |= BIT7; //pull sda low
			}
			else if (curr_bit == 1)
			{
				P2OUT &= ~BIT7; //pull sda high
			}
			trigger_scl();
		}
		//checking ack
		//i2c_delay();

		P2OUT &= ~BIT5; //pull scl high to receive ack
		i2c_delay();
		//i2c_delay();
		//i2c_delay();

		if ((P2IN&BIT6)  > 0)
		{
			ack = 1;
			break;
		}
	}

	if(stop != 0)
	{
		i2c_stop();
	}
	return ack;
}

// Returns the next <length> characters
void i2c_read(unsigned char length, unsigned char * rxData)
{
	int char_num = length;

	unsigned char curr_byte = 0;
	int i, j;

	P2OUT |= BIT5; //we have to be sure scl is pulled low to be able to receive the first bit
	i2c_delay();

	for (i = 0; i < char_num; i++)
	{

		curr_byte = 0;
		for (j = 7; j >= 0; j--)
		{
			P2OUT &= ~BIT5;//set scl high to read the current sda bit
			i2c_delay();

			curr_byte |= (((P2IN & BIT6)>>6) << j);//here we shift right by 6 to put our bit in the LSB position, and then we can shift it accordingly

			P2OUT |= BIT5; //we have to set scl is low before we read the next bit
			i2c_delay();
		}

		//send ack
		if (i < char_num - 1)//this is because as per the data sheet PCF8591, section 9.4, a slave will only know the end of transmission when receiving a NACK (SDA is high during the 9th scl pulse).
		{
			P2OUT |= BIT7; //pull sda low
		}

		trigger_scl();

		//release the sda line
		P2OUT &= ~BIT7; //pull sda high
		i2c_delay();

		rxData[i] = curr_byte;
	}

	i2c_stop();
}

// Return 0 if acknowledged, 1 if not.
void i2c_start(void)
{
	P2OUT &= ~(BIT5 | BIT7);//make sure both lines are high to initiate the strart condition
	i2c_delay();

	P2OUT |= BIT7; //pull sda low

	i2c_delay();

	P2OUT |= BIT5; //pull scl low

	i2c_delay();
}

void i2c_stop(void)
{
	P2OUT |= (BIT5 | BIT7);//make sure both lines are low to initiate the stop condition
	i2c_delay();

	P2OUT &= ~BIT5; //pull scl high

	i2c_delay();

	P2OUT |= BIT7; //pull sda high

	i2c_delay();
}


//This function will pull the scl high to allow the sda bit to be either written or read, and then pull it down again. The period is 10 us to achieve 100Kb/s
void trigger_scl(void)
{
	P2OUT &= ~BIT5; //pull scl high to send sda
	i2c_delay();

	P2OUT |= BIT5; //pull scl low
	i2c_delay();
}

void i2c_delay(void)
{
	__delay_cycles(5);//5us delay. Which is half a period

}

