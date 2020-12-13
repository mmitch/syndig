/*
 * compressor.c - sound compression to prevent clipping
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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "buffer.h"
#include "common.h"

#define NEUTRAL 1.0
#define FALLOFF 0.01

static bool compressing = false;
static BUFTYPE compression = NEUTRAL;

void compress_buffer() {
	BUFTYPE min = 0;
	BUFTYPE max = 0;
	for (uint16_t i = 0; i < BUFSIZE_STEREO; i++) {
		if (stereo_out[i] < min) {
			min = stereo_out[i];
		}
		if (stereo_out[i] > max) {
			max = stereo_out[i];
		}
	}
	max = MAX(max, -min);

	if (max > compression) {
		compression = max;
		if (! compressing) {
			printf("compressor kicked in at %f\n", compression);
			compressing = true;
		}
	} else {
		if (compression > NEUTRAL) {
			compression = MAX (compression - FALLOFF, NEUTRAL);
			if (compression == NEUTRAL) {
				compressing = false;
			}
		}
	}

	if (compressing) {
		for (uint16_t i = 0; i < BUFSIZE_STEREO; i++) {
			stereo_out[i] /= compression;
		}
	}
}
