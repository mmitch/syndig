/*
 * buffer.c - output sample buffer
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

#include <stdint.h>
#include <string.h>

#include "buffer.h"

BUFTYPE stereo_out[BUFSIZE * 2];
BUFTYPE silence[BUFSIZE * 2];

void init_sample_buffer() {
	for (uint16_t i = 0; i < BUFSIZE * 2; i++) {
		silence[i] = 0;
	}
	clear_stereo_buffer(stereo_out);
}

void clear_mono_buffer(BUFTYPE *buffer) {
	memcpy(buffer, silence, BUFBYTES_MONO);
}

void clear_stereo_buffer(BUFTYPE *buffer) {
	memcpy(buffer, silence, BUFBYTES_STEREO);
}
