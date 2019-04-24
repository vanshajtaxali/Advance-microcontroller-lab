/*
 * audio.h
 *
 *  Created on: 18.06.2015
 *      Author: sester
 */

#ifndef LIBS_AUDIO_H_
#define LIBS_AUDIO_H_



// You do not have to include flash.h, but I'd recommend it.
//#include "./flash.h"

//to initialize the PWM timer, and a secondary timer for the recording (and maybe playback) interrupt, and the ADC initialization
void audio_init(void);

// Read a single measurement.
unsigned char audio_single_read(void);

// Play a single measurement. If not stopped by calling something like
// audio_single_play(0), the played audio should not stop.
//void audio_single_play(unsigned char value); //replace by two timers

// Wait for a predefined time. Predefined by your sample rate, actually.
// (Intended to be used like this:
//   audio_single_play(sampleData[i]);
//   audio_single_wait();
//   audio_single_play(sampleData[i+1]);
// )
// Alternative: use a timer. In fact, using a timer would be much better
// and much more accurate.
void audio_single_wait();

// I'd recommend having a big buffer which you fill in the background
// and this single function which simply plays all data from the buffer
// until it reaches the end of it.
void audio_playBuffer(long int address);
void audio_recordBuffer(long int address);


#endif /* LIBS_AUDIO_H_ */
