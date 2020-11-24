/*
 * main.c - main program that orchestrates all other parts
 *          contains setup, teardown and synthesizer sound pipeline
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

#include <stdio.h>

#include "buffer.h"
#include "common.h"
#include "compressor.h"
#include "midi.h"
#include "oscillator.h"
#include "output.h"
#include "polyphony.h"

int main() {
	sound_output *sound = NULL;
	midi_input *midi = NULL;

	printf("%s is starting\n", PROGRAM_NAME);

	sound = get_sound_output();
	if (sound->open() != 0) {
		goto SHUTDOWN;
	}
	puts("sound output opened");

	midi = get_midi_input();
	if (midi->open() != 0) {
		goto SHUTDOWN;
	}
	puts("MIDI input opened");

	init_sample_buffer();
	puts("buffer initialized");

	init_polyphony();
	puts("polyphony initialized");

	init_envelopes();
	puts("envelopes initialized");

	init_oscillators();
	puts("oscillator initialized");

	while(1) {
		receive_midi(midi);
		clear_sample_buffer();
		run_oscillators();
		compress_buffer();
		sound->write();
	}

SHUTDOWN:
	if (midi != NULL) {
		midi->close();
		puts("MIDI input closed");
	}

	if (sound != NULL) {
		sound->close();
		puts("sound output closed");
	}

	return 0;
}
