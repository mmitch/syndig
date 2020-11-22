#include <string.h>

#include "output.h"

BUFTYPE samples[BUFSIZE];
BUFTYPE silence[BUFSIZE];

extern sound_output pulse_output;

static void init_samples() {
	for (int i = 0; i < BUFSIZE; i++) {
		silence[i] = 0;
	}
	clear_sample_buffer();
}

void clear_sample_buffer() {
	memcpy(samples, silence, BUFBYTES);
}

sound_output* get_sound_output() {
	init_samples();
	return &pulse_output;
}
