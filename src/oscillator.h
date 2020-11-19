#ifndef _OSCILLATOR_H_
#define _OSCILLATOR_H_

#include "output.h"

typedef enum {
	SQUARE,
	SAW_DOWN,
	SAW_UP,
} oscillator_type;

void init_oscillator();
void set_oscillator_frequency(float new_frequency);
void change_oscillator_type(oscillator_type new_type);
void run_oscillator(sound_output *sound);

#endif
