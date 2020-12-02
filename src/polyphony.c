/*
 * polyphony.c - polyphony implementation
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

#include <string.h>
#include <stdio.h>

#include "envelope.h"
#include "hertz.h"
#include "oscillator.h"
#include "polyphony.h"

#define NO_NOTE      255
#define LOWEST_NOTE  0
#define HIGHEST_NOTE 127

static polyphony_mode_t poly_mode = KILL_OLDEST;
static lane_id poly_history[POLYPHONY]; // contains every lane id exactly once; order matters
static lane_id round_robin;

static uint8_t last_note[POLYPHONY];

static void poly_history_set_newest(lane_id newest) {
	lane_id found;
	for (found = POLYPHONY-1; found > 0; found--) {
		if (poly_history[found] == newest) {
			break;
		}
	}
	if (found) {
		// move existing entry to front of queue
		bcopy(&poly_history[0], &poly_history[1], sizeof(lane_id) * found);
		poly_history[0] = newest;
	}
}

static lane_id poly_history_get_oldest() {
	return poly_history[POLYPHONY-1];
}

static lane_id find_free_lane() {
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		if (! envelope_is_running(lane)) {
			return lane;
		}
	}

	return NO_LANE;
}

static lane_id find_lane_with_note(uint8_t note) {
	for (lane_id id = 0; id < POLYPHONY; id++) {
		if (last_note[id] == note) {
			return id;
		}
	}
	return NO_LANE;
}

static lane_id find_lane_for(uint8_t note) {

	lane_id lane, i;

	if ((lane = find_lane_with_note(note)) != NO_LANE) {
		return lane;
	}

	if ((lane = find_free_lane()) != NO_LANE) {
		return lane;
	}
	
	switch (poly_mode) {

	case ROUND_ROBIN:
		round_robin++;
		if (round_robin >= POLYPHONY) {
			round_robin = 0;
		}
		lane = round_robin;
		break;

	case KILL_OLDEST:
		lane = poly_history_get_oldest();
		break;

	case KILL_LOWEST:
		note = HIGHEST_NOTE;
		for (i = 0; i < POLYPHONY; i++) {
			if (last_note[i] <= note && last_note[i] != NO_NOTE) {
				lane = i;
				note = last_note[lane];
			}
		}
		break;

	case KILL_HIGHEST:
		note = LOWEST_NOTE;
		for (i = 0; i < POLYPHONY; i++) {
			if (last_note[i] >= note && last_note[i] != NO_NOTE) {
				lane = i;
				note = last_note[lane];
			}
		}
		break;
	}

	printf("polyphony overflow: killing note %d\n", last_note[lane]);
	return lane;
}

void init_polyphony() {
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		poly_history[lane] = lane;
		last_note[lane] = NO_NOTE;
	}
}

void set_polyphony_mode(polyphony_mode new_mode) {
	poly_mode = new_mode.mode;
	printf("polyphony mode set to %s\n", new_mode.name);
}

void play_note(uint8_t note, float velocity) {
	lane_id lane = find_lane_for(note);
	poly_history_set_newest(lane);
	last_note[lane] = note;

	set_oscillator_frequency(lane, hertz[note]);
	trigger_envelope(lane, velocity);
}

void stop_note(uint8_t note) {
	lane_id lane = find_lane_with_note(note);
	if (lane != NO_LANE) {
		release_envelope(lane);
	}
}

void stop_all_notes() {
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		if (envelope_is_running(lane)) {
			release_envelope(lane);
		}
	}
}
