/*
 * input.h - MIDI input configuration interface
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

#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "common.h"

typedef enum {
	NOTE_ON,
	NOTE_OFF,
	PROGRAM_CHANGE,
	CONTROL_CHANGE,
} midi_event_type;

typedef struct {
	uint8_t note;
	uint8_t velocity;
} midi_data_note_on;

typedef struct {
	uint8_t note;
	uint8_t velocity;
} midi_data_note_off;

typedef struct {
	uint8_t program;
} midi_data_program_change;

typedef struct {
	uint8_t param;
	uint8_t value;
} midi_data_control_change;

typedef struct {
	midi_event_type type;
	channel_id      channel;
	union {
		midi_data_note_on        note_on;
		midi_data_note_off       note_off;
		midi_data_program_change program_change;
		midi_data_control_change control_change;
	} data;
} midi_event;

typedef int         (*midi_open_fn )(void);
typedef midi_event* (*midi_read_fn )(void);
typedef int         (*midi_close_fn)(void);

typedef struct {
	midi_open_fn  open;
	midi_read_fn  read;
	midi_close_fn close;
} midi_input;

midi_input* get_midi_input();

#endif
