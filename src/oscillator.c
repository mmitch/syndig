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

#include <math.h>
#include <stdlib.h>

#include "buffer.h"
#include "channel.h"
#include "envelope.h"
#include "oscillator.h"

typedef struct {
	channel_id channel;
	float phase;
	frequency frequency;
	float wavelength;
	float sample_hold_last;
	uint8_t sample_hold_index;
	BUFTYPE buffer[BUFSIZE_MONO];
} oscillator;

#define PI             3.14159265358979323846
#define SIN_PI_FOURTH  0.70710678118654752440

#define WAVELET_LENGTH 8

typedef float (*nextval_fn)(uint8_t);

static float wavelet_square_25[WAVELET_LENGTH] = { 0, 1, 0.99, -1, -0.99, -0.98, -0.96, -0.94 };

static float wavelet_square_50[WAVELET_LENGTH] = { 0, 1, 0.99, 0.97, 0, -1, -0.99, -0.98 };

static float wavelet_saw_down[WAVELET_LENGTH] = { 1, 1-(2/7.0), 1-(4/7.0), 1-(6/7.0), 1-(8/7.0), 1-(10/7.0), 1-(12/7.0), -1};

static float wavelet_saw_up[WAVELET_LENGTH] = { -1, (2/7.0)-1, (4/7.0)-1, (6/7.0)-1, (8/7.0)-1, (10/7.0)-1, (12/7.0)-1, 1};

static float wavelet_triangle[WAVELET_LENGTH] = { 0, 0.5, 1, 0.5, 0, -0.5, -1, -0.5 };

static float wavelet_sine[WAVELET_LENGTH] = { 0, SIN_PI_FOURTH, 1, SIN_PI_FOURTH, 0, -SIN_PI_FOURTH, -1, -SIN_PI_FOURTH };

static float wavelet_noise[WAVELET_LENGTH] = { 0.751, -0.4151, 0.3319, 0.685, -0.1561, -3.786, -0.0093, -0.5787 };

static float wavelet_double_pulse[WAVELET_LENGTH] = { 0, 1, 0, 1, 0, -1, 0, -1 };

static oscillator osc[POLYPHONY];

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

static float wavelet_saw_down_nextval(uint8_t index) {
	return wavelet_saw_down[index];
}

static float wavelet_saw_up_nextval(uint8_t index) {
	return wavelet_saw_up[index];
}

static float wavelet_triangle_nextval(uint8_t index) {
	return wavelet_triangle[index];
}

static float wavelet_sine_nextval(uint8_t index) {
	return wavelet_sine[index];
}

static float wavelet_noise_nextval(uint8_t index) {
	return wavelet_noise[index];
}

static float wavelet_double_pulse_nextval(uint8_t index) {
	return wavelet_double_pulse[index];
}

static float sample_and_hold(oscillator *o, nextval_fn nextval) {
	float   wavelength_eighth = o->wavelength / 8.0;
	float   hold       = o->sample_hold_last;
	uint8_t index      = o->sample_hold_index;
	float   phase      = o->phase;
	BUFTYPE *buffer    = o->buffer;

	for (int i = 0; i < BUFSIZE_MONO; i++) {
		phase++;
		while (phase >= wavelength_eighth) {
			phase -= wavelength_eighth;

			index++;
			if (index >= WAVELET_LENGTH) {
				index = 0;
			}

			hold = nextval(index);
		}
		buffer[i] = hold;
	}

	o->sample_hold_last  = hold;
	o->sample_hold_index = index;

	return phase;
}

BUFTYPE* run_oscillator(lane_id lane) {
	oscillator *o = &osc[lane];

	BUFTYPE *buffer  = o->buffer;
	float phase      = o->phase;
	float wavelength = o->wavelength;

	switch (ch_config[o->channel].osc) {

	case SQUARE:
	{
		float wavelength_half = wavelength / 2.0;
		for (int i = 0; i < BUFSIZE_MONO; i++) {
			phase++;
			while (phase >= wavelength) {
				phase -= wavelength;
			}
			buffer[i] = (phase < wavelength_half) ? 1 : -1;
		}
		break;
	}

	case SAW_DOWN:
		for (int i = 0; i < BUFSIZE_MONO; i++) {
			phase++;
			while (phase >= wavelength) {
				phase -= wavelength;
			}
			buffer[i] = 1 - (2 * phase / wavelength);
		}
		break;

	case SAW_UP:
		for (int i = 0; i < BUFSIZE_MONO; i++) {
			phase++;
			while (phase >= wavelength) {
				phase -= wavelength;
			}
			buffer[i] = -1 + (2 * phase / wavelength);
		}
		break;

	case TRIANGLE:
	{
		float wavelength_half = wavelength / 2.0;
		for (int i = 0; i < BUFSIZE_MONO; i++) {
			phase++;
			while (phase >= wavelength) {
				phase -= wavelength;
			}
			if (phase < wavelength_half) {
				buffer[i] =  1 - (4 * phase / wavelength);
			} else {
				buffer[i] = -1 + (4 * (phase - wavelength_half) / wavelength);
			}
		}
		break;
	}

	case SINE:
		for (int i = 0; i < BUFSIZE_MONO; i++) {
			phase++;
			while (phase >= wavelength) {
				phase -= wavelength;
			}
			buffer[i] = sinf( phase * 2 * PI / wavelength );
		}
		break;

	case IMPULSE:
	{
		static float last_impulse;
		float wavelength_half = wavelength / 2.0;
		for (int i = 0; i < BUFSIZE_MONO; i++) {
			phase++;
			while (phase >= wavelength) {
				phase -= wavelength;
			}
			float impulse = (phase < wavelength_half) ? 1 : -1;
			if (impulse != last_impulse) {
				last_impulse = impulse;
				buffer[i] = impulse;
			} else {
				buffer[i] = 0;
			}
		}
		break;
	}

	case NOISE:
		phase = sample_and_hold(o, random_nextval);
		break;

	case WAVELET_SQUARE_25:
		phase = sample_and_hold(o, wavelet_square_25_nextval);
		break;

	case WAVELET_SQUARE_50:
		phase = sample_and_hold(o, wavelet_square_50_nextval);
		break;

	case WAVELET_SAW_DOWN:
		phase = sample_and_hold(o, wavelet_saw_down_nextval);
		break;

	case WAVELET_SAW_UP:
		phase = sample_and_hold(o, wavelet_saw_up_nextval);
		break;

	case WAVELET_TRIANGLE:
		phase = sample_and_hold(o, wavelet_triangle_nextval);
		break;

	case WAVELET_SINE:
		phase = sample_and_hold(o, wavelet_sine_nextval);
		break;

	case WAVELET_NOISE:
		phase = sample_and_hold(o, wavelet_noise_nextval);
		break;

	case WAVELET_DOUBLE_PULSE:
		phase = sample_and_hold(o, wavelet_double_pulse_nextval);
		break;

	}

	o->phase = phase;

	return buffer;
}

void init_oscillators() {
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		osc[lane].wavelength = 1;
		set_oscillator_frequency(lane, 440);
	}
	for (channel_id channel = 0; channel < CHANNELS; channel++) {
		ch_config[channel].osc = SQUARE;
	}
}

void set_oscillator_frequency(lane_id lane, frequency new_frequency) {
	osc[lane].frequency = new_frequency;
	float relative_phase = osc[lane].phase / osc[lane].wavelength;
	osc[lane].wavelength = SAMPLERATE / osc[lane].frequency;
	osc[lane].phase = relative_phase * osc[lane].wavelength;
}

void set_oscillator_channel(lane_id lane, channel_id channel) {
	osc[lane].channel = channel;
}

void set_oscillator_type(channel_id channel, oscillator_type type) {
	ch_config[channel].osc = type;
}
