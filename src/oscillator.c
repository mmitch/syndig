#include <stdint.h>

#include "envelope.h"
#include "oscillator.h"
#include "output.h"

#define BUFSIZE 128
#define BUFTYPE float
#define BUFBYTES (BUFSIZE * sizeof(BUFTYPE))
static BUFTYPE samples[BUFSIZE];

// FIXME: get samplerate from somewhere else
#define SAMPLERATE 44100.0

static float frequency;
static float wavelength;
static float wavelength_half;

static float phase = 0;

void init_oscillator() {
	set_oscillator_frequency(440);
}

void set_oscillator_frequency(float new_frequency) {
	frequency = new_frequency;
	wavelength = SAMPLERATE / frequency;
	wavelength_half = wavelength / 2.0;
}

void run_oscillator(sound_output *sound) {
	for (int i = 0; i < BUFSIZE; i++) {
		phase++;
		while (phase >= wavelength) {
			phase -= wavelength;
		}
		samples[i] = ((phase < wavelength_half) ? 1 : -1) * envelope_nextval();
	}
	sound->write(&samples, BUFBYTES);
}
