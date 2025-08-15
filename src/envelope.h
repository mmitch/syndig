/*
 * envelope.h - envelope interface
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

#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

#include <stdbool.h>

#include "buffer.h"
#include "common.h"

typedef struct {
	float attack_rate;
	float decay_rate;
	float sustain_level;
	float release_rate;
} adsr;

void  init_envelopes(void);
void  set_envelope_attack(channel_id channel, uint16_t attack_ms);
void  set_envelope_decay(channel_id channel, uint16_t decay_ms);
void  set_envelope_sustain(channel_id channel, float sustain);
void  set_envelope_release(channel_id channel, uint16_t release_ms);
void  trigger_envelope(lane_id id, channel_id channel);
void  release_envelope(lane_id id);
void  stop_envelope(lane_id id);
BUFTYPE* run_envelope(lane_id id);
bool  envelope_is_running(lane_id id);

#endif
