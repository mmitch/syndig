/*
 * output_pulse.c - PulseAudio sound driver
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
#include <unistd.h>

#include <pulse/error.h>
#include <pulse/simple.h>

#include "buffer.h"
#include "common.h"
#include "output.h"

static pa_simple *pulse_handle;

static int pulse_open()
{
	pa_sample_spec pulse_spec;
	pulse_spec.format = PA_SAMPLE_FLOAT32;
	pulse_spec.rate = SAMPLERATE;
	pulse_spec.channels = 1;

	int err;
	pulse_handle = pa_simple_new(NULL, PROGRAM_NAME, PA_STREAM_PLAYBACK, NULL, PROGRAM_NAME, &pulse_spec, NULL, NULL, &err);
	if (!pulse_handle) {
		fprintf(stderr, "pulse: %s\n", pa_strerror(err));
		return -1;
	}

	return 0;
}

static ssize_t pulse_write()
{
	return pa_simple_write(pulse_handle, &samples, BUFBYTES, NULL);
}

static int pulse_close()
{
	pa_simple_free(pulse_handle);
	return 0;
}

sound_output pulse_output = {
	.open = pulse_open,
	.write = pulse_write,
	.close = pulse_close,
};
