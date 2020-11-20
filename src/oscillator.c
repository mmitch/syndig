#include <string.h>

#include "common.h"
#include "envelope.h"
#include "oscillator.h"
#include "output.h"

#define BUFSIZE 128
#define BUFTYPE float
#define BUFBYTES (BUFSIZE * sizeof(BUFTYPE))
static BUFTYPE samples[BUFSIZE];
static BUFTYPE silence[BUFSIZE];

// FIXME: get samplerate from somewhere else
#define SAMPLERATE 44100.0

typedef struct {
	oscillator_type type;
	float phase;
	float frequency;
	float wavelength;
} oscillator;

static oscillator osc[POLYPHONY];

static oscillator_type type = SQUARE;

static void run_oscillator(id id) {
	if (! envelope_is_running(id)) {
		return;
	}

	oscillator *o = &osc[id];

	switch (type) {

	case SQUARE:
	{
		float wavelength_half = o->wavelength / 2.0;
		for (int i = 0; i < BUFSIZE; i++) {
			o->phase++;
			while (o->phase >= o->wavelength) {
				o->phase -= o->wavelength;
			}
			samples[i] += ((o->phase < wavelength_half) ? 1 : -1) * envelope_nextval();
		}
		break;
	}

	case SAW_DOWN:
		for (int i = 0; i < BUFSIZE; i++) {
			o->phase++;
			while (o->phase >= o->wavelength) {
				o->phase -= o->wavelength;
			}
			samples[i] += (1 - (o->phase / o->wavelength) * 2) * envelope_nextval();
		}
		break;

	case SAW_UP:
		for (int i = 0; i < BUFSIZE; i++) {
			o->phase++;
			while (o->phase >= o->wavelength) {
				o->phase -= o->wavelength;
			}
			samples[i] += (-1 + (o->phase / o->wavelength) * 2) * envelope_nextval();
		}
		break;

	}
}

void init_oscillators() {
	for (id id; id < POLYPHONY; id++) {
		osc[id].type       = type;
		osc[id].wavelength = 1;
		set_oscillator_frequency(id, 440);
	}

	for (int i = 0; i < BUFSIZE; i++) {
		silence[i] = 0;
	}
}

void set_oscillator_frequency(id id, float new_frequency) {
	osc[id].frequency = new_frequency;
	float relative_phase = osc[id].phase / osc[id].wavelength;
	osc[id].wavelength = SAMPLERATE / osc[id].frequency;
	osc[id].phase = relative_phase * osc[id].wavelength;
}

void change_oscillator_type(oscillator_type new_type) {
	type = new_type;
}

void run_oscillators(sound_output *sound) {
	memcpy(samples, silence, BUFBYTES);
	for (id id; id < POLYPHONY; id++) {
		run_oscillator(id);
	}
	sound->write(&samples, BUFBYTES);
}
