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

void  init_envelopes();
void  set_envelope_attack(channel channel, uint16_t attack_ms);
void  set_envelope_decay(channel channel, uint16_t decay_ms);
void  set_envelope_sustain(channel channel, float sustain);
void  set_envelope_release(channel channel, uint16_t release_ms);
void  trigger_envelope(channel channel, lane_id id);
void  release_envelope(lane_id id);
void  stop_envelope(lane_id id);
BUFTYPE* run_envelope(lane_id id);
bool  envelope_is_running(lane_id id);

#endif
