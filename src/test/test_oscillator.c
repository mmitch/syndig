/*
 * test_oscillator.c - unit tests of oscillator.c
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

#include "../oscillator.c"

#include "../thirdparty/greatest.h"

#include "mock/mock_envelope.h"

BUFTYPE samples[BUFSIZE];

static void reset_oscillator()
{
	type = SQUARE;
}

static void setup() {
	reset_oscillator();

	reset_envelope_mocks();
	FFF_RESET_HISTORY()
}

TEST default_oscillator_type_is_square() {
	// given

	// when
	setup();

	// then
	ASSERT_EQ(SQUARE, type);

	PASS();
}

TEST change_oscillator_type_works() {
	// given
	setup();

	// when
	change_oscillator_type(SAW_DOWN);

	// then
	ASSERT_EQ(SAW_DOWN, type);

	PASS();
}

TEST change_oscillator_type_does_not_change_type_on_lanes() {
	// given
	setup();

	// when
	change_oscillator_type(SAW_UP);

	// then
	ASSERT_EQ(SQUARE, osc[0].type);

	PASS();
}

TEST init_oscillators_resets_all_lanes() {
	// given
	setup();

	// when
	init_oscillators();

	// then
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		ASSERT_EQ(SQUARE, osc[lane].type);
		ASSERT_EQ(440.0,  osc[lane].frequency);
		ASSERT_IN_RANGE(SAMPLERATE/440.0, osc[lane].wavelength, 0.0001);
	}

	PASS();
}

TEST init_oscillators_changes_type_on_all_lanes() {
	// given
	setup();
	change_oscillator_type(SAW_DOWN);

	// when
	init_oscillators();

	// then
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		ASSERT_EQ(SAW_DOWN, osc[lane].type);
	}

	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	time_t t;
	srand((unsigned) time(&t));

	GREATEST_MAIN_BEGIN();

	SHUFFLE_TESTS(rand(), {
			RUN_TEST(default_oscillator_type_is_square);
			RUN_TEST(change_oscillator_type_works);
			RUN_TEST(change_oscillator_type_does_not_change_type_on_lanes);
			RUN_TEST(init_oscillators_resets_all_lanes);
			RUN_TEST(init_oscillators_changes_type_on_all_lanes);
		});

	GREATEST_MAIN_END();
}
