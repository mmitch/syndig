#include <stdint.h>

#include "envelope.h"
#include "oscillator.h"
#include "output.h"

#define BUFSIZE 128
#define BUFTYPE float
#define BUFBYTES (BUFSIZE * sizeof(BUFTYPE))
static BUFTYPE samples[BUFSIZE];

static float frequency = 440;
static float phase     = 0;

void set_oscillator_frequency(float new_frequency) {
	frequency = new_frequency;
}

void run_oscillator(sound_output *sound) {

	// FIXME: get samplerate from somewhere else
	float wavelength = 44100.0 / frequency;
	float wavelength_half = wavelength / 2.0;
	for (int i = 0; i < BUFSIZE; i++) {
		phase++;
		while (phase >= wavelength) {
			phase -= wavelength;
		}
		samples[i] = ((phase < wavelength_half) ? 1 : -1) * envelope_nextval();
	}
	sound->write(&samples, BUFBYTES);
}