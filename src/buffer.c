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

#include <string.h>

#include "buffer.h"

BUFTYPE samples[BUFSIZE];
BUFTYPE silence[BUFSIZE];

void init_sample_buffer() {
	for (int i = 0; i < BUFSIZE; i++) {
		silence[i] = 0;
	}
	clear_sample_buffer();
}

void clear_sample_buffer() {
	memcpy(samples, silence, BUFBYTES);
}
