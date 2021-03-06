/*
 * oscillator.h - oscillator interface
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

#ifndef _OSCILLATOR_H_
#define _OSCILLATOR_H_

#include "buffer.h"
#include "common.h"

typedef enum {
	SQUARE,
	SAW_DOWN,
	SAW_UP,
	TRIANGLE,
	SINE,
	NOISE,
	IMPULSE,
	WAVELET_SQUARE_25,
	WAVELET_SQUARE_50,
	WAVELET_SAW_DOWN,
	WAVELET_SAW_UP,
	WAVELET_TRIANGLE,
	WAVELET_SINE,
	WAVELET_NOISE,
	WAVELET_DOUBLE_PULSE,
} oscillator_type;

void init_oscillators();
void set_oscillator_frequency(lane_id id, frequency new_frequency);
void set_oscillator_channel(lane_id id, channel_id channel);
void set_oscillator_type(channel_id channel, oscillator_type type);
BUFTYPE* run_oscillator(lane_id id);

#endif
