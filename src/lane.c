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

typedef struct {
	channel_id channel;
	float      velocity;
} lane_t;

static lane_t l[POLYPHONY];

static void run_lane(lane_id lane) {

	lane_t         *la = &l[lane];
	channel_config *ch = &ch_config[la->channel];
	
	BUFTYPE *osc = run_oscillator(lane);
	BUFTYPE *env = run_envelope(lane);

	// FIXME: extract mixer and amplifier
	float amp = la->velocity * ch->vol;
	float amp_l = amp * ch->vol_left;
	float amp_r = amp * ch->vol_right;
	uint8_t i;
	uint16_t j;
	for (i = j = 0; i < BUFSIZE_MONO; i++) {
		BUFTYPE val = osc[i] * env[i];
		stereo_out[j++] += val * amp_l;
		stereo_out[j++] += val * amp_r;
	}
}

void init_lanes() {
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		l[lane].velocity = 0;
		l[lane].channel  = 0;
	}
}

void trigger_lane(channel_id channel, lane_id lane, frequency frequency, float velocity) {
	l[lane].velocity = velocity;
	l[lane].channel  = channel;
	set_oscillator_channel(lane, channel);
	set_oscillator_frequency(lane, frequency);
	trigger_envelope(lane, channel);
}

void run_lanes() {
	clear_buffer(stereo_out);
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		if (envelope_is_running(lane)) {
			run_lane(lane);
		}
	}
	compress_buffer();
}
