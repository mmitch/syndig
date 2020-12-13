/*
 * test_lane.c - unit tests of lane.c
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

#include "../lane.c"

#include "../thirdparty/greatest.h"

#include "mock/mock_buffer.h"
#include "mock/mock_compressor.h"
#include "mock/mock_envelope.h"
#include "mock/mock_oscillator.h"

#include "../channel.c" // contains no code, so no mock needed

BUFTYPE stereo_out[BUFSIZE * 2];

static void setup() {
	init_lanes();

	for (uint8_t i = 0; i < BUFSIZE; i++) {
		stereo_out[i] = 0;
	}

	reset_buffer_mocks();
	reset_compressor_mocks();
	reset_envelope_mocks();
	reset_oscillator_mocks();
	FFF_RESET_HISTORY()
}

TEST init_lanes_resets_all_lanes() {
	// given
	setup();

	l[3].velocity = 0.5;
	l[2].channel  = 9;

	// when
	init_lanes();

	// then
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		ASSERT_EQ(0, l[lane].velocity);
		ASSERT_EQ(0, l[lane].channel);
	}

	PASS();
}

TEST trigger_lane_sets_velocity_and_channel() {
	// given
	setup();

	lane_id    lane = 4;
	channel_id ch   = 2;
	float      freq = 2500;
	float      vel  = 0.5;

	// when
	trigger_lane(ch, lane, freq, vel);

	// then
	ASSERT_EQ(vel, l[lane].velocity);
	ASSERT_EQ( ch, l[lane].channel);

	PASS();
}

TEST trigger_lane_configures_oscillator() {
	// given
	setup();

	lane_id    lane = 1;
	channel_id ch   = 3;
	float      freq = 335;
	float      vel  = 0.8;

	// when
	trigger_lane(ch, lane, freq, vel);

	// then
	ASSERT_EQ(   1, set_oscillator_frequency_fake.call_count);
	ASSERT_EQ(lane, set_oscillator_frequency_fake.arg0_val); // lane
	ASSERT_EQ(freq, set_oscillator_frequency_fake.arg1_val); // frequency

	ASSERT_EQ(   1, set_oscillator_channel_fake.call_count);
	ASSERT_EQ(lane, set_oscillator_channel_fake.arg0_val); // lane
	ASSERT_EQ(  ch, set_oscillator_channel_fake.arg1_val); // channel

	PASS();
}

TEST trigger_lane_triggers_envelope() {
	// given
	setup();

	lane_id    lane = 7;
	channel_id ch   = 13;
	float      freq = 4800;
	float      vel  = 0.3;

	// when
	trigger_lane(ch, lane, freq, vel);

	// then
	ASSERT_EQ(   1, trigger_envelope_fake.call_count);
	ASSERT_EQ(lane, trigger_envelope_fake.arg0_val); // lane
	ASSERT_EQ(  ch, trigger_envelope_fake.arg1_val); // channel

	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	time_t t;
	srand((unsigned) time(&t));

	GREATEST_MAIN_BEGIN();

	SHUFFLE_TESTS(rand(), {
			RUN_TEST(init_lanes_resets_all_lanes);
			RUN_TEST(trigger_lane_sets_velocity_and_channel);
			RUN_TEST(trigger_lane_configures_oscillator);
			RUN_TEST(trigger_lane_triggers_envelope);
		});

	GREATEST_MAIN_END();
}
