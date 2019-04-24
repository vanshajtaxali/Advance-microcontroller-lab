/*
 * MSP430 Timer Tutorial Example Code 3
 * Anthony Scranney
 * www.Coder-Tronics.com
 * August 2014
 *
 * PWM example using both TimerA_0 and TimerA_1 to increase and decrease
 * the duty cycle, which in turn alters the brightness of the on board green LED
 */

#define NO_TEMPLATE_UART
#include <templateEMP.h>
#include "libs/audio.h"
//#include "libs/flash.h"
//#include "libs/sounddata.h"
#include "libs/adac.h"
#include "libs/LCD.h"
#include "libs/i2c.h"
#include "libs/spi.h"

/*
 * For the first time, you do not have to follow the header files[1] when
 * implementing your code, you should rather take this as a (rough)
 * recommendation on how you *could* do it.
 * So here's a rather lengthy comment about how this is supposed to work:
 * You should gather samples at high speed (try to get at least 20 kHz
 * sample rate). You may either poll the samples or use an interrupt together
 * with continous sampling.
 * Then, whenever you got a sample, put it into some buffer. I'd recommend a
 * ring buffer, but really, it's up to you.
 * Whenever that buffer is filled to, say, 50 % you trigger the flash_write
 * command to transfer the data in the background to the flash memory. When
 * the data got transfered successfully, remove the written data from the
 * buffer.
 *
 * On the playback:
 * Get data from flash, put it into some kind of buffer, play back the data
 * and whenever you played like 50 % of your buffer, refill it in the
 * background. Just make sure that your playback-speed matches the recording
 * speed (approximately at least) [2]. You may want to use a timer for a more
 * precise timing.
 *
 * [1] Except for spi.h. You should (roughly) follow that header file when
 *     implementing the spi.c. You don't have to, but grading will be more
 *     complicated it you do not follow it.
 * [2] I highly recommend to use a hardware timer for a precise timing.
 *     If you do so, make sure to implement it as a library ... :-)
 */

/*** Global Variable ***/

void DisplayMenu();

int I2C_SPI;

void main(void) {
	initMSP();
	// TODO (sesters): Set up the watchdog and the speed here.
	WDTCTL = WDTPW + WDTHOLD;		// Stop watchdog timer

	// if you change this!
	BCSCTL1 = CALBC1_16MHZ;
	// Set DCO step + modulation
	DCOCTL = CALDCO_16MHZ;

	// TODO (sesters): Add your initialization here.

	unsigned char status = 0;
	//unsigned char x;
	unsigned char y;
	unsigned char pressed = 0;
	unsigned char valuesFromADC[4];	//we only need 3, but it is required to pass an array of size 4 at least

	DisplayMenu();

	//-------------------------------------
	//I2C_SPI = 2;
	//flash_init();
	//unsigned char bytes_to_write[10];
	//int i;
	//for (i = 0; i < 10; ++i) {
	//	bytes_to_write[i] = i + 11;
	//}

	//flash_write(0x0A, 10, bytes_to_write);

	//unsigned char bytes_to_read[10];
	//flash_read(0x0A, 10, bytes_to_read);

	//---------------------------------------------
	I2C_SPI = 1;
	audio_init();

	status = adac_init();

	while (status)
		;

	//__delay_cycles(500);

	if (status > 0) {

		lcd_cursorSet(0, 0);

		__delay_cycles(500);
		lcd_putText("Error.... ");
	}

	int menuSelection = 0;
	const int refreshRate = 500;
	int refreshCounter = 0;
	int lastPress = 0;

	//int samplesRecorded = 0;
	//int sampleToDelete = 0;
	int sampleToPlay = 0;
	int sampleToRecord = 0;
	while (1) {
		// TODO (sesters): Implement exercise 4 here.
		I2C_SPI = 1;
		status = adac_read(valuesFromADC);
		//x = valuesFromADC[0];    //x axis on the joystick
		y = valuesFromADC[1];    //y axis on the joystick
		pressed = valuesFromADC[2];    //joystick button

		//status = adac_write(x); //remove later. this shoud be for movement of cursor

		if (y > 0xC0) {
			menuSelection = 1;
		} else if (y < 0x40) {
			menuSelection = 0;
		}

		refreshCounter++;
		if (refreshCounter == refreshRate) {
			lcd_cursorSet(15, menuSelection);
			refreshCounter = 0;
		}

		if (pressed > 0 && lastPress == 0) {
			I2C_SPI = 2;
			//select action
			if (menuSelection == 0) {
				lcd_cursorShow(1);
				//make a new recording
				switch (sampleToRecord) {
				case 0:
					audio_playBuffer(0x000000);
					break;
				case 1:
					audio_playBuffer(0x080000);
					break;
				case 2:
					audio_playBuffer(0x100000);
					break;
				case 3:
					audio_playBuffer(0x180000);
					break;
				default:
					break;
				}

				sampleToRecord++;
				if (sampleToRecord == 4)
					sampleToRecord = 0;

				//__delay_cycles(60000000); //500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.
				lcd_cursorBlink(1);	//blink cursor

			} else {
				lcd_enable(0);
				switch (sampleToPlay) {
				case 0:
					audio_playBuffer(0x000000);
					break;
				case 1:
					audio_playBuffer(0x080000);
					break;
				case 2:
					audio_playBuffer(0x100000);
					break;
				case 3:
					audio_playBuffer(0x180000);
					break;
				default:
					break;
				}

				sampleToPlay++;
				if (sampleToPlay == 4)
					sampleToPlay = 0;

				//__delay_cycles(60000000);//500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.
				lcd_enable(1);

			}

		}
		if (pressed == 0) {
			lastPress = 0;
		}
	}
}

void DisplayMenu() {
	lcd_init();

	__delay_cycles(20000000);//500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	lcd_clear();

	__delay_cycles(20000000);//500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	//lcd_cursorSet(0, 0);

	//__delay_cycles(500000);	//500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	//__delay_cycles(500);

	lcd_putText("Record sample");

	__delay_cycles(20000000);//500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	//__delay_cycles(500);

	lcd_cursorSet(0, 1);

	__delay_cycles(20000000);//500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	//__delay_cycles(500);

	lcd_putText("Play sample");

	__delay_cycles(20000000);//500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	//__delay_cycles(500);

	lcd_cursorSet(15, 0);

	__delay_cycles(20000000);//500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	lcd_cursorBlink(1);	//blink cursor

	__delay_cycles(20000000);//500ms delay. And as it was already mentioned, there are some overheads, so, not quite exact.

	//__delay_cycles(500);
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void) {

	//while (!(IFG2 & UCB0RXIFG))
	//;// USCI_B0 TX buffer ready?
	//unsigned char receivedChar;
	if (I2C_SPI == 1) {
		// check for NACK
		if (UCB0STAT & UCNACKIFG) {
			// send a stop condition if slave sends NACK and clear the NACK flag
			UCB0CTL1 |= UCTXSTP;
			UCB0STAT &= ~UCNACKIFG;
			i2c_nackReceived = 1;
			// exit low power mode
			__low_power_mode_off_on_exit();
		}

	} else if (I2C_SPI == 2) {
		if (rwFlag == 1) {
			if (spi_dataLength > 1) {
				*spi_dataIBuffer = UCB0RXBUF;
				++spi_dataIBuffer;
				--spi_dataLength;
				UCB0TXBUF = 0x00;
			} else {
				*spi_dataIBuffer = UCB0RXBUF;
				__low_power_mode_off_on_exit();
			}

		} else {
			if (spi_dataLength > 1) {
				//receivedChar = UCB0RXBUF;
				++spi_dataOBuffer;
				--spi_dataLength;
				UCB0TXBUF = *spi_dataOBuffer;

			} else {
				IFG2 &= ~UCB0RXIFG;
				__low_power_mode_off_on_exit();
			}
		}
	}
}
