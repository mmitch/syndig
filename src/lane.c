/*
 * lane.c - sound pipeline implementation
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

#include "lane.h"

#include "buffer.h"
#include "channel.h"
#include "compressor.h"
#include "envelope.h"
#include "oscillator.h"

static float velocity[POLYPHONY];

static void run_lane(lane_id lane) {

	BUFTYPE *osc = run_oscillator(lane);
	BUFTYPE *env = run_envelope(lane);

	// FIXME: extract mixer and amplifier
	float amp = velocity[lane];
	for (int i = 0; i < BUFSIZE; i++) {
		BUFTYPE val = osc[i] * env[i] * amp;
		samples[i] += val;
	}
}

void init_lanes() {
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		velocity[lane] = 0;
	}
}

void trigger_lane(lane_id lane, channel_id channel, frequency frequency, float new_velocity) {
	velocity[lane] = new_velocity;
	set_oscillator_frequency(lane, frequency);
	set_oscillator_type(lane, ch_config[channel].osc);
	trigger_envelope(channel, lane);
}

void run_lanes() {
	clear_sample_buffer(samples);
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		if (envelope_is_running(lane)) {
			run_lane(lane);
		}
	}
	compress_buffer();
}
