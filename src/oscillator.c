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

static float phase           = 1;
static float frequency       = 1;
static float wavelength      = 1;
static float wavelength_half = 1;

static enum oscillator_type type = SQUARE;

void init_oscillator() {
	set_oscillator_frequency(440);
}

void set_oscillator_frequency(float new_frequency) {
	frequency = new_frequency;

	float relative_phase = phase / wavelength;

	wavelength = SAMPLERATE / frequency;
	wavelength_half = wavelength / 2.0;

	phase = relative_phase * wavelength;
}

void run_oscillator(sound_output *sound) {
	switch (type) {

	case SQUARE:
		for (int i = 0; i < BUFSIZE; i++) {
			phase++;
			while (phase >= wavelength) {
				phase -= wavelength;
			}
			samples[i] = ((phase < wavelength_half) ? 1 : -1) * envelope_nextval();
		}
		break;

	case SAW_DOWN:
		for (int i = 0; i < BUFSIZE; i++) {
			phase++;
			while (phase >= wavelength) {
				phase -= wavelength;
			}
			samples[i] = (1 - (phase / wavelength) * 2) * envelope_nextval();
		}
		break;

	case SAW_UP:
		for (int i = 0; i < BUFSIZE; i++) {
			phase++;
			while (phase >= wavelength) {
				phase -= wavelength;
			}
			samples[i] = (-1 + (phase / wavelength) * 2) * envelope_nextval();
		}
		break;

	}

	sound->write(&samples, BUFBYTES);
}
