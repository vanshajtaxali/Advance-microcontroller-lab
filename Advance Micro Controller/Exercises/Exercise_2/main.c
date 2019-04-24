/*
 * Exercise sheet 2
 *
 *  Created on: 21.05.2015
 *      Author: Hassan Gamaleldin
 */

#include <templateEMP.h>
//#include <msp430g2553.h>
#include "libs/adac.h"

int main(void) {
    initMSP();
    // TODO (sesters): Add your initialization here.
    // clear PxSEL register
	P2SEL &= ~BIT6;
	P2SEL &= ~BIT7;

	P2DIR |= (BIT5 | BIT7);    // set
	P2DIR &= ~(BIT4 | BIT6);    //unset to have P2.4 and P2.6 as inputs

	P3DIR |= (BIT6); // set as output

    adac_init();

    //-----------------------------------------------------------------------------


    unsigned char val = 0;
    unsigned char valuesFromADC[4];//we only need 3, but it is required to pass an array of size 4 at least

    unsigned char status = adac_write(val);


    unsigned char x, y;
    unsigned char pressed = 0;

    while (1)
    {
		if (pressed == 0) //if the  button is not pressed, then make a tone, otherwise, don't.
		{
			P3OUT |= BIT6;
			// the following will decide which note to play depending on our y value. The periods were acquired from https://www.arduino.cc/en/Tutorial/PlayMelody
			if (y < 32) { //3830 is the period for the c note, we will delay half of it while our pin is high, then the other half when the pin is low
				__delay_cycles(1915);
			} else if (y < 64) { //3400 is the period for the d note, we will delay half of it while our pin is high, then the other half when the pin is low
				__delay_cycles(1700);

			} else if (y < 96) { //3038 is the period for the e note, we will delay half of it while our pin is high, then the other half when the pin is low
				__delay_cycles(1519);

			} else if (y < 128) { //2864 is the period for the f note, we will delay half of it while our pin is high, then the other half when the pin is low
				__delay_cycles(1432);

			} else if (y < 160) { //2550 is the period for the g note, we will delay half of it while our pin is high, then the other half when the pin is low
				__delay_cycles(1275);

			} else if (y < 192) { //2272 is the period for the a note, we will delay half of it while our pin is high, then the other half when the pin is low
				__delay_cycles(1136);

			} else if (y < 224) { //2028 is the period for the b note, we will delay half of it while our pin is high, then the other half when the pin is low
				__delay_cycles(1014);

			} else if (y < 256) { //1912 is the period for the C note, we will delay half of it while our pin is high, then the other half when the pin is low
				__delay_cycles(956);
			}
		}
		//here we don't need to check for the press, since we lower the pin anyways
		P3OUT &= ~BIT6;//buzzer pin low

		//for all the following values, we will deduct 950us as buffer for all the time taken for the other fucntion calls in the loop.
		//This is not completly accurate, but it is very close, and sounds very close to the tones when there is nothing else in the loop.
		if (y < 32) { //3830 is the period for the c note, we will delay half of it while our pin is high, then the other half when the pin is low
			__delay_cycles(965);
		} else if (y < 64) { //3400 is the period for the d note, we will delay half of it while our pin is high, then the other half when the pin is low
			__delay_cycles(750);

		} else if (y < 96) { //3038 is the period for the e note, we will delay half of it while our pin is high, then the other half when the pin is low
			__delay_cycles(569);

		} else if (y < 128) { //2864 is the period for the f note, we will delay half of it while our pin is high, then the other half when the pin is low
			__delay_cycles(482);

		} else if (y < 160) { //2550 is the period for the g note, we will delay half of it while our pin is high, then the other half when the pin is low
			__delay_cycles(325);

		} else if (y < 192) { //2272 is the period for the a note, we will delay half of it while our pin is high, then the other half when the pin is low
			__delay_cycles(186);

		} else if (y < 224) { //2028 is the period for the b note, we will delay half of it while our pin is high, then the other half when the pin is low
			__delay_cycles(64);

		} else if (y < 256) { //1912 is the period for the C note, we will delay half of it while our pin is high, then the other half when the pin is low
			__delay_cycles(6);
		}

        status = adac_read(valuesFromADC);

        //Test to see if everything is working as it should. It will be commented in the final delivery
        //if (status != 0){
        //	break;
        //}

        x = valuesFromADC[0];    //x axis on the joystick
		y = valuesFromADC[1];    //y axis on the joystick
		pressed = valuesFromADC[2];    //joystick button


		status = adac_write(x);

		//Test to see if everything is working as it should. It will be commented in the final delivery
		//if (status != 0) {
		//	break;
		//}
    	// TODO (sesters): Implement your solution here
    }
}
/*
void SetLines(void)//this is a temprorary solution to set the sda and scl lines to 1
{
	 P2DIR |= (BIT4 | BIT6);

	 P2OUT |= (BIT4 | BIT6);

	 P2DIR &= ~(BIT4 | BIT6);
}
*/

//This is how it was before making some changes to the following if statement
/*
if (pressed == 0) //if the  button is not pressed, then make a tone, otherwise, don't.
		{
			// the following will decide which note to play depending on our y value. The periods were acquired from https://www.arduino.cc/en/Tutorial/PlayMelody
			if (y < 32) { //3830 is the period for the c note, we will delay half of it while our pin is high, then the other half when the pin is low
				P3OUT |= BIT6;
				__delay_cycles(1915);
				P3OUT &= ~BIT6;
				__delay_cycles(1915);
			} else if (y < 64) { //3400 is the period for the d note, we will delay half of it while our pin is high, then the other half when the pin is low
				P3OUT |= BIT6;
				__delay_cycles(1700);
				P3OUT &= ~BIT6;
				__delay_cycles(1700);

			} else if (y < 96) { //3038 is the period for the e note, we will delay half of it while our pin is high, then the other half when the pin is low
				P3OUT |= BIT6;
				__delay_cycles(1519);
				P3OUT &= ~BIT6;
				__delay_cycles(1519);

			} else if (y < 128) { //2864 is the period for the f note, we will delay half of it while our pin is high, then the other half when the pin is low
				P3OUT |= BIT6;
				__delay_cycles(1432);
				P3OUT &= ~BIT6;
				__delay_cycles(1432);

			} else if (y < 160) { //2550 is the period for the g note, we will delay half of it while our pin is high, then the other half when the pin is low
				P3OUT |= BIT6;
				__delay_cycles(1275);
				P3OUT &= ~BIT6;
				__delay_cycles(1275);

			} else if (y < 192) { //2272 is the period for the a note, we will delay half of it while our pin is high, then the other half when the pin is low
				P3OUT |= BIT6;
				__delay_cycles(1136);
				P3OUT &= ~BIT6;
				__delay_cycles(1136);

			} else if (y < 224) { //2028 is the period for the b note, we will delay half of it while our pin is high, then the other half when the pin is low
				P3OUT |= BIT6;
				__delay_cycles(1014);
				P3OUT &= ~BIT6;
				__delay_cycles(1014);

			} else if (y < 256) { //1912 is the period for the C note, we will delay half of it while our pin is high, then the other half when the pin is low
				P3OUT |= BIT6;
				__delay_cycles(956);
				P3OUT &= ~BIT6;
				__delay_cycles(956);
			}
		}
*/
