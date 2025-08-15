/*
 * output.h - sound driver configuration interface
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

#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <unistd.h>

typedef int     (*sound_open_fn )(void);
typedef ssize_t (*sound_write_fn)(void);
typedef int     (*sound_close_fn)(void);

typedef const struct {
	sound_open_fn  open;
	sound_write_fn write;
	sound_close_fn close;
} sound_output;

sound_output* get_sound_output(void);

#endif
