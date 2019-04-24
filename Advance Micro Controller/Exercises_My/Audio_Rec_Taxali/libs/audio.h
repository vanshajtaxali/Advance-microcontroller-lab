/***************************************************************************//**
 * @file    audio.h
 * @author  <VANSHAJ TAXALI>
 * @Email   <vanshajtaxali94@gmail.com>
 * @date    <02-07-18>
 *
 * @brief   <brief description>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/
#ifndef LIBS_AUDIO_H_
#define LIBS_AUDIO_H_

#include "./isl.h"

#include "./flash.h"

#define Resolution 0xFF  //8-bit resolution

// Read a single measurement.
unsigned char audio_single_read();

// Play a single measurement. If not stopped by calling something like
// audio_single_play(0), the played audio should not stop.
void audio_single_play(unsigned char value);

// Wait for a predefined time. Predefined by your sample rate, actually.

void audio_single_wait();

// A big buffer which we fill in the background
// and this single function which simply plays all data from the buffer
// until it reaches the end of it.
void audio_playBuffer();



#endif /* LIBS_AUDIO_H_ */
