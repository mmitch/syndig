#ifndef _OSCILLATOR_H_
#define _OSCILLATOR_H_

#include <stdbool.h>

#include "output.h"

typedef struct {
	bool active;
	float frequency;
} oscillator_input;

void run_oscillator(sound_output *sound);

#endif
