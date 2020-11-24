/*
 * polyphony.h - polyphony interface
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

#ifndef _POLYPHONY_H_
#define _POLYPHONY_H_

#include "common.h"

typedef enum {
	KILL_OLDEST,
	KILL_LOWEST,
	KILL_HIGHEST,
	ROUND_ROBIN,
} polyphony_mode_t;

typedef struct {
	polyphony_mode_t mode;
	char*            name;
} polyphony_mode;

void init_polyphony();
void set_polyphony_mode(polyphony_mode new_mode);
void play_note(uint8_t note, float velocity);
void stop_note(uint8_t note);

#endif
