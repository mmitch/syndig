#include "buffer.h"
#include "compressor.h"
#include "envelope.h"
#include "oscillator.h"
#include "output.h"

typedef struct {
	oscillator_type type;
	float phase;
	float frequency;
	float wavelength;
} oscillator;

static oscillator osc[POLYPHONY];

static oscillator_type type = SQUARE;

static void run_oscillator(lane_id lane) {
	if (! envelope_is_running(lane)) {
		return;
	}

	oscillator *o = &osc[lane];

	switch (type) {

	case SQUARE:
	{
		float wavelength_half = o->wavelength / 2.0;
		for (int i = 0; i < BUFSIZE; i++) {
			o->phase++;
			while (o->phase >= o->wavelength) {
				o->phase -= o->wavelength;
			}
			samples[i] += ((o->phase < wavelength_half) ? 1 : -1) * envelope_nextval(lane);
		}
		break;
	}

	case SAW_DOWN:
		for (int i = 0; i < BUFSIZE; i++) {
			o->phase++;
			while (o->phase >= o->wavelength) {
				o->phase -= o->wavelength;
			}
			samples[i] += (1 - (o->phase / o->wavelength) * 2) * envelope_nextval(lane);
		}
		break;

	case SAW_UP:
		for (int i = 0; i < BUFSIZE; i++) {
			o->phase++;
			while (o->phase >= o->wavelength) {
				o->phase -= o->wavelength;
			}
			samples[i] += (-1 + (o->phase / o->wavelength) * 2) * envelope_nextval(lane);
		}
		break;

	}
}

void init_oscillators() {
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		osc[lane].type       = type;
		osc[lane].wavelength = 1;
		set_oscillator_frequency(lane, 440);
	}
}

void set_oscillator_frequency(lane_id lane, float new_frequency) {
	osc[lane].frequency = new_frequency;
	float relative_phase = osc[lane].phase / osc[lane].wavelength;
	osc[lane].wavelength = SAMPLERATE / osc[lane].frequency;
	osc[lane].phase = relative_phase * osc[lane].wavelength;
}

void change_oscillator_type(oscillator_type new_type) {
	type = new_type;
}

void run_oscillators(sound_output *sound) {
	clear_sample_buffer();
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		run_oscillator(lane);
	}
	compress_buffer();
	sound->write();
}
