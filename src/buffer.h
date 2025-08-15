/*
 * buffer.h - output sample buffer interface
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

#ifndef _BUFFER_H_
#define _BUFFER_H_

#define BUFSIZE_MONO   128
#define BUFSIZE_STEREO BUFSIZE_MONO * 2
#define BUFTYPE float
#define BUFBYTES_MONO   (BUFSIZE_MONO   * sizeof(BUFTYPE))
#define BUFBYTES_STEREO (BUFSIZE_STEREO * sizeof(BUFTYPE))
extern BUFTYPE stereo_out[BUFSIZE_STEREO];

#define SAMPLERATE 44100.0

void init_buffer(void);
void clear_buffer(BUFTYPE *buffer);

#endif
