#ifndef _OSCILLATOR_H_
#define _OSCILLATOR_H_

#include "common.h"
#include "output.h"

typedef enum {
	SQUARE,
	SAW_DOWN,
	SAW_UP,
} oscillator_type;

void init_oscillators();
void set_oscillator_frequency(lane_id id, float new_frequency);
void change_oscillator_type(oscillator_type new_type);
void run_oscillators(sound_output *sound);

#endif
