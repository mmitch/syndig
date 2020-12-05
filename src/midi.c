/*
 * midi.c - MIDI event handler
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

#include "common.h"
#include "envelope.h"
#include "input.h"
#include "oscillator.h"
#include "polyphony.h"

#define MAX_MIDI  127.0

#define LAST_INDEX(arr)          ((sizeof(arr) / (sizeof(arr[0]))) - 1)
#define CLAMP_TO_MAP(val, map)   { if (val > LAST_INDEX(map)) { val = 0; } }

/*
 * stretch MIDI range [0..127] into bigger envelope range [0ms..~5000ms]
 * this function is roughly linear from 0->0 to about 64->900
 * and then goes on more exponentially to about 127->5000
 * gnuplot> plot [0:127] x**1.62,1.064**x,x**1.62+1.064**x
 */
#define STRETCH(x)  (powf((x), 1.62) + powf( 1.064, (x)))

#define to_struct(x) { .mode = x, .name = #x }
static polyphony_mode poly_map[] = {
	to_struct(KILL_OLDEST),
	to_struct(KILL_LOWEST),
	to_struct(KILL_HIGHEST),
	to_struct(ROUND_ROBIN),
};

static oscillator_type program_map[] = {
	SQUARE,
	SAW_DOWN,
	SAW_UP,
	TRIANGLE,
	SINE,
	NOISE,
	WAVELET_SQUARE_25,
	WAVELET_SQUARE_50,
	WAVELET_SAW_DOWN,
	WAVELET_SAW_UP,
	WAVELET_TRIANGLE,
	WAVELET_SINE,
	WAVELET_NOISE,
	WAVELET_DOUBLE_PULSE,
};

void receive_midi(midi_input *midi) {
	midi_event *event;
	while ((event = midi->read()) != NULL) {
		switch (event->type) {

		case NOTE_ON:
		{
			play_note(
				event->channel,
				event->data.note_on.note,
				event->data.note_on.velocity / MAX_MIDI);
			break;
		}

		case NOTE_OFF:
		{
			stop_note(event->channel, event->data.note_on.note);
			break;
		}

		case PROGRAM_CHANGE:
		{
			uint8_t program = event->data.program_change.program;
			CLAMP_TO_MAP(program, program_map);
			set_oscillator_type(event->channel, program_map[program]);
			break;
		}

		case CONTROL_CHANGE:
		{
			uint8_t value = event->data.control_change.value;
			switch (event->data.control_change.param) {
			case 3:
				CLAMP_TO_MAP(value, poly_map);
				set_polyphony_mode(poly_map[value]);
				break;

			case 72:
				set_envelope_release(event->channel, STRETCH(value));
				break;
				
			case 73:
				set_envelope_attack(event->channel, STRETCH(value));
				break;
				
			case 75:
				set_envelope_sustain(event->channel, value / MAX_MIDI);
				break;

			case 80:
				set_envelope_decay(event->channel, STRETCH(value));
				break;

			case 120:
				stop_all_sound();
				break;

			case 123:
				stop_all_notes();
				break;

			}
			break;
		}
		}
	}
}
