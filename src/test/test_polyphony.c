/*
 * test_polyphony.c - unit test of polyphony.c
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

#include "../polyphony.c"

#include "../thirdparty/greatest.h"

#include "mock/mock_envelope.h"
#include "mock/mock_oscillator.h"

const double hertz[128] = { 111.0, 222.0, 333.0 };

static void setup() {
	reset_envelope_mocks();
	reset_oscillator_mocks();
		
	FFF_RESET_HISTORY()
}

TEST play_notes_plays_on_first_lane() {
	// given
	setup();

	// when
	play_note(3, 40, 90);

	// then
	ASSERT_EQ(0, poly_history[0]);
	ASSERT_EQ(40, last_note[0]);

	ASSERT_EQ(1,         set_oscillator_frequency_fake.call_count);
	ASSERT_EQ(0,         set_oscillator_frequency_fake.arg0_val); // lane
	ASSERT_EQ(hertz[40], set_oscillator_frequency_fake.arg1_val); // frequency

	ASSERT_EQ(1,      set_oscillator_type_fake.call_count);
	ASSERT_EQ(0,      set_oscillator_type_fake.arg0_val); // lane
	ASSERT_EQ(SQUARE, set_oscillator_type_fake.arg1_val); // type

	ASSERT_EQ(1,  trigger_envelope_fake.call_count);
	ASSERT_EQ(3,  trigger_envelope_fake.arg0_val); // channel
	ASSERT_EQ(0,  trigger_envelope_fake.arg1_val); // lane
	ASSERT_EQ(90, trigger_envelope_fake.arg2_val); // velocity

	PASS();
}

TEST init_polyphony_resets_last_notes() {
	// given
	setup();

	// when
	init_polyphony();

	// then
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		ASSERT_EQ(NO_NOTE, last_note[lane]);
	}
	
	PASS();
}

TEST init_polyphony_resets_poly_history() {
	// given
	setup();

	// when
	init_polyphony();

	// then
	for (uint8_t i = 0; i < POLYPHONY; i++) {
		ASSERT_EQ(i, poly_history[i]);
	}
	
	PASS();
}

TEST stop_all_notes_sets_all_running_envelopes_to_release() {
	// given
	setup();
	bool envelope_status[] = { true, false, false, true, false };
	SET_RETURN_SEQ(envelope_is_running, envelope_status, 5);

	// when
	stop_all_notes();

	// then
	ASSERT_EQ(2, release_envelope_fake.call_count);
	ASSERT_EQ(0, release_envelope_fake.arg0_history[0]); // lane
	ASSERT_EQ(3, release_envelope_fake.arg0_history[1]); // lane

	PASS();
}

TEST stop_all_sound_sets_all_running_envelopes_to_off() {
	// given
	setup();
	bool envelope_status[] = { false, true, true, false };
	SET_RETURN_SEQ(envelope_is_running, envelope_status, 4);

	// when
	stop_all_sound();

	// then
	ASSERT_EQ(2, stop_envelope_fake.call_count);
	ASSERT_EQ(1, stop_envelope_fake.arg0_history[0]); // lane
	ASSERT_EQ(2, stop_envelope_fake.arg0_history[1]); // lane

	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	time_t t;
	srand((unsigned) time(&t));

	GREATEST_MAIN_BEGIN();

	SHUFFLE_TESTS(rand(), {
			RUN_TEST(play_notes_plays_on_first_lane);
			RUN_TEST(init_polyphony_resets_last_notes);
			RUN_TEST(init_polyphony_resets_poly_history);
			RUN_TEST(stop_all_notes_sets_all_running_envelopes_to_release);
			RUN_TEST(stop_all_sound_sets_all_running_envelopes_to_off);
		});

	GREATEST_MAIN_END();
}
