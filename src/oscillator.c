/*
 * oscillator.c - oscillator implementation
 *
 * Copyright (C) 2020  Christian Garbs <mitch@cgarbs.de>
 * Licensed under GNU GPL v3 (or later)
 *
 * This file is part of syndig, a simple software synthesizer written in C.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

#include "buffer.h"
#include "envelope.h"
#include "oscillator.h"

typedef struct {
	oscillator_type type;
	float phase;
	frequency frequency;
	float wavelength;
	float sample_hold_last;
	uint8_t sample_hold_index;
} oscillator;

#define WAVELET_LENGTH 8

typedef float (*nextval_fn)(uint8_t);

static float wavelet_square_25[WAVELET_LENGTH] = { 0, 1, 0.99, -1, -0.99, -0.98, -0.96, -0.94 };

static float wavelet_square_50[WAVELET_LENGTH] = { 0, 1, 0.99, 0.97, 0, -1, -0.99, -0.98 };

static float wavelet_noise[WAVELET_LENGTH] = { 0.751, -0.4151, 0.3319, 0.685, -0.1561, -3.786, -0.0093, -0.5787 };

static oscillator osc[POLYPHONY];

static oscillator_type type = SQUARE;

static float random_nextval(uint8_t index) {
	UNUSED(index);
	return (double)rand() / (double)(RAND_MAX/2) - 1.0;
}

static float wavelet_square_25_nextval(uint8_t index) {
	return wavelet_square_25[index];
}

static float wavelet_square_50_nextval(uint8_t index) {
	return wavelet_square_50[index];
}

static float wavelet_noise_nextval(uint8_t index) {
	return wavelet_noise[index];
}

static void sample_and_hold(lane_id lane, oscillator *o, nextval_fn nextval) {
	float wavelength_eighth = o->wavelength / 8.0;
	float hold = o->sample_hold_last;
	uint8_t index = o->sample_hold_index;

	for (int i = 0; i < BUFSIZE; i++) {
		o->phase++;
		while (o->phase >= wavelength_eighth) {
			o->phase -= wavelength_eighth;

			index++;
			if (index >= WAVELET_LENGTH) {
				index = 0;
			}

			hold = nextval(index);
		}
		samples[i] += hold * envelope_nextval(lane);
	}

	o->sample_hold_last  = hold;
	o->sample_hold_index = index;
}

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

	case TRIANGLE:
	{
		float wavelength_half = o->wavelength / 2.0;
		for (int i = 0; i < BUFSIZE; i++) {
			o->phase++;
			while (o->phase >= o->wavelength) {
				o->phase -= o->wavelength;
			}
			if (o->phase < wavelength_half) {
				samples[i] += (1 - (o->phase / o->wavelength) * 4) * envelope_nextval(lane);
			} else {
				samples[i] += (-1 + ((o->phase - wavelength_half) / o->wavelength) * 4) * envelope_nextval(lane);
			}
		}
		break;
	}

	case NOISE:
		sample_and_hold(lane, o, random_nextval);
		break;

	case WAVELET_SQUARE_25:
		sample_and_hold(lane, o, wavelet_square_25_nextval);
		break;

	case WAVELET_SQUARE_50:
		sample_and_hold(lane, o, wavelet_square_50_nextval);
		break;

	case WAVELET_NOISE:
		sample_and_hold(lane, o, wavelet_noise_nextval);
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

void set_oscillator_frequency(lane_id lane, frequency new_frequency) {
	osc[lane].frequency = new_frequency;
	float relative_phase = osc[lane].phase / osc[lane].wavelength;
	osc[lane].wavelength = SAMPLERATE / osc[lane].frequency;
	osc[lane].phase = relative_phase * osc[lane].wavelength;
}

void change_oscillator_type(oscillator_type new_type) {
	type = new_type;
}

void run_oscillators() {
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		run_oscillator(lane);
	}
}
