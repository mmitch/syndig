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

#include "buffer.h"
#include "common.h"
#include "envelope.h"

#define STRETCH   10

#define MS_TO_TICKS(x)  ((x) * SAMPLERATE / 1000.0)

typedef struct {
	float attack_rate;
	float decay_rate;
	float sustain_level;
	float release_rate;
} adsr;

typedef enum {
	ATTACK,
	DECAY,
	SUSTAIN,
	RELEASE,
	OFF
} state;

typedef struct {
	state state;
	float value;
	float velocity;
	adsr  *params;
} envelope;

static adsr     params[CHANNELS];
static envelope env[POLYPHONY];

void init_envelopes() {
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		env[lane].state    = OFF;
		env[lane].value    = 0;
		env[lane].velocity = 0;
	}
	for (channel channel = 0; channel < CHANNELS; channel++) {
		set_envelope_attack(channel, 30);
		set_envelope_decay(channel, 50);
		set_envelope_sustain(channel, 0.5);
		set_envelope_release(channel, 30);
	}
}

void trigger_envelope(channel channel, lane_id lane, float velocity) {
	env[lane].state    = ATTACK;
	env[lane].velocity = velocity;
	env[lane].params   = &params[channel];
}

void release_envelope(lane_id lane) {
	env[lane].state = RELEASE;
}

void stop_envelope(lane_id lane) {
	env[lane].state = OFF;
}

float envelope_nextval(lane_id lane) {
	envelope *e = &env[lane];
	adsr     *p = e->params;
	
	if (e->state == OFF) {
		return 0;
	}

	if (e->state == ATTACK) {
		if (p->attack_rate == 0 || e->value >= e->velocity) {
			e->value = e->velocity;
			e->state = DECAY;
		} else {
			e->value = MIN( e->value + (p->attack_rate * e->velocity), e->velocity);
		}
	}
	if (e->state == DECAY) {
		if (p->decay_rate == 0 || e->value <= p->sustain_level * e->velocity) {
			e->value = p->sustain_level * e->velocity;
			e->state = SUSTAIN;
		} else {
			e->value = MAX( e->value - (p->decay_rate  * e->velocity), p->sustain_level * e->velocity);
		}
	}
	if (e->state == RELEASE) {
		if (p->release_rate == 0 || e->value <= 0) {
			e->value = 0;
			e->state = OFF;
		} else {
			e->value = MAX( e->value - (p->release_rate * e->velocity), 0);
		}
	}

	return e->value;
}

bool envelope_is_running(lane_id lane) {
	return env[lane].state != OFF;
}

/*
 *           1
 *  rate = -----
 *         ticks
 */
void set_envelope_attack(channel channel, uint16_t attack_ms) {
	if (attack_ms == 0) {
		params[channel].attack_rate = 0;
	} else {
		params[channel].attack_rate = 1.0 / MS_TO_TICKS(attack_ms);
	}
}

void set_envelope_decay(channel channel, uint16_t decay_ms) {
	if (decay_ms == 0) {
		params[channel].decay_rate = 0;
	} else {
		params[channel].decay_rate = 1.0 / MS_TO_TICKS(decay_ms);
	}
}

void set_envelope_sustain(channel channel, float sustain) {
	params[channel].sustain_level = sustain;
}

void set_envelope_release(channel channel, uint16_t release_ms) {
	if (release_ms == 0) {
		params[channel].release_rate = 0;
	} else {
		params[channel].release_rate = 1.0 / MS_TO_TICKS(release_ms);
	}
}
