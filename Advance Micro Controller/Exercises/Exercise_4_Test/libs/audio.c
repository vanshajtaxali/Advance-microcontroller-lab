/*
 * audio.c
 *
 *  Created on: 16.07.2016
 *      Author: Hassan Gamaleldin
 */

#include <msp430g2553.h>
#include "./isl.h"
///#include "sounddata.h"
#include "./flash.h"

unsigned char recordingBuffer[200];
//unsigned char backgroundgBuffer[20000];

unsigned char playBackBuffer[200];

int startPlaying = 0;
int startRecording = 0;

int playIndex = 0;
int recordIndex = 0;

long int recordingAddress;

//int playFlag = 0;

void audio_init(void) {

	/*** GPIO Set-Up ***/
	//P1DIR |= BIT6;
	P3DIR |= BIT6;					// P3.6 set as output
	P3SEL |= BIT6;					// P3.6 selected Timer0_A Out1 output
	//P3DIR &= ~BIT0;
	P1SEL |= BIT0;
	//P1SEL |= BIT6;					// P1.6 selected Timer0_A Out2 output

	//ADC set-up
	ADC10CTL1 = INCH_0 + CONSEQ_0;// + ADC10DIV_3;         // Channel 3, ADC10CLK/3
	ADC10CTL0 = ADC10SHT_3 + ADC10ON; // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
	ADC10AE0 |= BIT0;

	/*** Timer0_A Set-Up ***/
	TA0CCR0 = 0xFF;						// PWM period
	TA0CCR2 = 0;						// TA0CCR2 PWM duty cycle
	TA0CCTL2 = OUTMOD_3;					// TA0CCR2 output mode = reset/set
	TA0CTL = TASSEL_2 + MC_1 + ID_0;	// SMCLK, Up Mode (Counts to TA0CCR0)

	/*** Timer1_A Set-Up ***/
	//20KHz
	TA1CCR0 = 800 - 1;					// Counter value
	TA1CCTL0 = CCIE;				// Enable Timer1_A interrupts
	TA1CTL = TASSEL_2 + MC_1;		// SMCLK, Up Mode (Counts to TA1CCR0)

	unsigned char status = 0;

	status = isl_init();
	while (status)
		;

	//__delay_cycles(500);

	//status = isl_set(0xA8);
	//while (status)
	//	;

	_BIS_SR(GIE);

}

// Read a single measurement.
unsigned char audio_single_read(void) {

	ADC10CTL0 &= ~ENC;				// Disable Conversion
	while (ADC10CTL1 & BUSY)
		;
	//	;		// Wait if ADC10 busy

	//ADC10SA = (int)adc;				// Transfers data to next array (DTC auto increments address)
	ADC10CTL0 |= ENC + ADC10SC;			// Sampling and conversion start
	//__bis_SR_register(GIE);// Low Power Mode 0 with interrupts enabled

	unsigned char ADC_value = ADC10MEM >> 2;// Assigns the value held in ADC10MEM to the integer called ADC_value

	return ADC_value;
}

// Play a single measurement. If not stopped by calling something like
// audio_single_play(0), the played audio should not stop.
void audio_single_play(unsigned char value) {

	TA0CCR2 = value;
}

// Wait for a predefined time. Predefined by your sample rate, actually.
// (Intended to be used like this:
//   audio_single_play(sampleData[i]);
//   audio_single_wait();
//   audio_single_play(sampleData[i+1]);
// )
// Alternative: use a timer. In fact, using a timer would be much better
// and much more accurate.
//This is replaced by two timers.
//void audio_single_wait() {

//}

// I'd recommend having a big buffer which you fill in the background
// and this single function which simply plays all data from the buffer
// until it reaches the end of it.
void audio_playBuffer(long int address) {//will be used to play an audio from the flash

	int i;

	for (i = 0; i < 100; ++i) {
		flash_read(address, 200, playBackBuffer);
		address += 200;
		startPlaying = 1;
		while (startPlaying > 0)
				;
	}
}

void audio_recordBuffer(long int address) {

	int i;
	for (i = 0; i < 100; ++i) {

		recordingAddress = address;
		address += 200;
		startRecording = 1;
		while (startRecording > 0)
			;
	}
}

#pragma vector=TIMER1_A0_VECTOR     // Timer1 A0 interrupt service routine
__interrupt void Timer1_A0(void) {

	//if (startRecording > 0) {
	//	recordingBuffer[recordIndex++] = audio_single_read();

		//if (recordIndex == 150) {
			//flash_write(recordingAddress, 255, recordingBuffer);
		//}
	//	if (recordIndex == 200) {
	//		flash_write(recordingAddress, 200, recordingBuffer);
	//		recordIndex = 0;
	//		startRecording = 0;
	//	}

	//}

	//unsigned char val = audio_single_read();

	//if (playFlag == 0)
	//{

	audio_single_play(val);//Here I played back what I got from my mic what is in val

	//if (startPlaying == 0) {
	//	audio_single_play(0);
	//} else {
	//	audio_single_play(playBackBuffer[playIndex++]);
	//	if (playIndex == 10000) {
	//		playIndex = 0;
	//		startPlaying = 0;
	//	}

	//}

	//audio_single_play( sounddata_data[index++]);
	//	playFlag = 1;
	//}
	//else
	//{
	//	playFlag = 0;
	//}

	//if (index == 10000)
	//	index = 0;

	//
	//
	//TA0CCR1 += IncDec_PWM;			// Increase or decrease duty cycle

	//if( TA0CCR1 > 254 || TA0CCR1 < 2 )	// Reverse direction if it falls within values
	// IncDec_PWM = -IncDec_PWM;
}
