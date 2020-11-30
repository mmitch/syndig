/*
 * common.h - common definitions and global configuration
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

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>

#define PROGRAM_NAME "syndig"
#define POLYPHONY 16


typedef double frequency;

typedef uint8_t lane_id;
// max id + 1 is unknown/not found
#define NO_LANE POLYPHONY

// don't warn on intentionally unused variables
#define UNUSED(x) (void)(x)

// sys/param.h has these too and gets included by PulseAudio
#ifndef MIN
#define MIN(x, y)  ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y)  ((x) > (y) ? (x) : (y))
#endif

#endif
