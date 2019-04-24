/***************************************************************************//**
 * @file    audio.c
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <02-07-18>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include <msp430g2553.h>
#include "audio.h"

extern unsigned char *PlaySampleData;
extern unsigned char singleWaitFlag, sampleRateFlag;
unsigned char sampleCount = 0;
unsigned char singleRead;

// Read a single measurement.
unsigned char audio_single_read()
{
	// Read a single measurement
	singleRead = PlaySampleData[sampleCount];

	// return measurement read
	return  singleRead;
}

// Play a single measurement. If not stopped by calling something like
// audio_single_play(0), the played audio should not stop.
void audio_single_play(unsigned char value)
{
	if (value)
	{
		// no interrupt flag set yet
		sampleRateFlag = 0;
		// wait for interrupt flag
		while(sampleRateFlag == 0);
		// put data out to the microphone
		TA0CCR2 = audio_single_read();
		// next data
		audio_single_wait();
	}
	else
	{	// stop audio
		TA0CCR2 = 0;
	}
}


void audio_single_wait()
{
	__delay_cycles(50);
}



void audio_playBuffer()
{
	unsigned char sampleCount = 0;

	// Keep playing tones stored in buffer
	do
	{
		// play a single tone
		audio_single_play(1);

		// next measurement to be read
		PlaySampleData++;
		// increment sample counter
		sampleCount++;
	}while(sampleCount != 255);
}

