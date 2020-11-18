#include <stdint.h>

#include "oscillator.h"
#include "output.h"

#define BUFSIZE 128
#define BUFTYPE uint16_t
#define BUFBYTES (BUFSIZE * sizeof(BUFTYPE))
static BUFTYPE silence[BUFSIZE];
static BUFTYPE square[BUFSIZE];


oscillator_input oscillator = {
	.active    = false,
	.frequency = 440,
};

static float phase = 0;
void run_oscillator(sound_output *sound) {

	if (!oscillator.active) {
		sound->write(&silence, BUFBYTES);
		return;
	}

	// FIXME: get samplerate from somewhere else
	float wavelength = 44100.0 / oscillator.frequency;
	float wavelength_half = wavelength / 2.0;
	for (int i = 0; i < BUFSIZE; i++) {
		phase++;
		while (phase >= wavelength) {
			phase -= wavelength;
		}
		square[i] = (phase < wavelength_half) ? 1 << 14 : 1;
	}
	sound->write(&square, BUFBYTES);
}
