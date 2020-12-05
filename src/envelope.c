/*
 * envelope.c - envelope implementation
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

#include <stdint.h>

#include "envelope.h"

#include "buffer.h"
#include "common.h"
#include "channel.h"

#define STRETCH   10

#define MS_TO_TICKS(x)  ((x) * SAMPLERATE / 1000.0)

typedef enum {
	ATTACK,
	DECAY,
	SUSTAIN,
	RELEASE,
	OFF
} state;

typedef struct {
	state      state;
	float      value;
	channel_id channel;
	BUFTYPE    values[BUFSIZE];
} envelope;

static envelope env[POLYPHONY];

void init_envelopes() {
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		env[lane].state    = OFF;
		env[lane].value    = 0;
	}
	for (channel_id channel = 0; channel < CHANNELS; channel++) {
		set_envelope_attack(channel, 30);
		set_envelope_decay(channel, 50);
		set_envelope_sustain(channel, 0.5);
		set_envelope_release(channel, 30);
	}
}

void trigger_envelope(channel_id channel, lane_id lane) {
	env[lane].state   = ATTACK;
	env[lane].channel = channel;
}

void release_envelope(lane_id lane) {
	env[lane].state = RELEASE;
}

void stop_envelope(lane_id lane) {
	env[lane].state = OFF;
}

BUFTYPE* run_envelope(lane_id lane) {
	envelope *e = &env[lane];
	adsr     *p = &ch_config[e->channel].env;

	float value = e->value;
	float state = e->state;
	BUFTYPE *values = e->values;
	
	for (int i = 0; i < BUFSIZE; i++) {

		if (state == OFF) {

			value = 0;

		} else {

			if (state == ATTACK) {
				if (p->attack_rate == 0 || value >= 1) {
					value = 1;
					state = DECAY;
				} else {
					value = MIN(value + p->attack_rate, 1);
				}
			}

			if (state == DECAY) {
				if (p->decay_rate == 0 || value <= p->sustain_level) {
					value = p->sustain_level;
					state = SUSTAIN;
				} else {
					value = MAX(value - p->decay_rate, p->sustain_level);
				}
			}
			if (state == RELEASE) {
				if (p->release_rate == 0 || value <= 0) {
					value = 0;
					state = OFF;
				} else {
					value = MAX(value - p->release_rate, 0);
				}
			}
		}

		values[i] = value;
	}

	e->value = value;
	e->state = state;

	return values;
}

bool envelope_is_running(lane_id lane) {
	return env[lane].state != OFF;
}

/*
 *           1
 *  rate = -----
 *         ticks
 */
void set_envelope_attack(channel_id channel, uint16_t attack_ms) {
	if (attack_ms == 0) {
		ch_config[channel].env.attack_rate = 0;
	} else {
		ch_config[channel].env.attack_rate = 1.0 / MS_TO_TICKS(attack_ms);
	}
}

void set_envelope_decay(channel_id channel, uint16_t decay_ms) {
	if (decay_ms == 0) {
		ch_config[channel].env.decay_rate = 0;
	} else {
		ch_config[channel].env.decay_rate = 1.0 / MS_TO_TICKS(decay_ms);
	}
}

void set_envelope_sustain(channel_id channel, float sustain) {
	ch_config[channel].env.sustain_level = sustain;
}

void set_envelope_release(channel_id channel, uint16_t release_ms) {
	if (release_ms == 0) {
		ch_config[channel].env.release_rate = 0;
	} else {
		ch_config[channel].env.release_rate = 1.0 / MS_TO_TICKS(release_ms);
	}
}
