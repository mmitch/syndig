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

#include "buffer.h"
#include "envelope.h"
#include "oscillator.h"

typedef struct {
	oscillator_type type;
	float phase;
	frequency frequency;
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
