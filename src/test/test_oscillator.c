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

static void setup() {
	init_oscillators();

	reset_envelope_mocks();
	FFF_RESET_HISTORY()
}

TEST init_oscillators_resets_all_lanes() {
	// given
	setup();

	osc[2].frequency = 13;
	osc[1].wavelength = 17;
	osc[0].type = SAW_DOWN;

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

TEST set_oscillator_type_affects_only_given_lane() {
	// given
	setup();

	// when
	set_oscillator_type(CHANNELS - 1, SAW_DOWN);

	// then
	for (lane_id lane = 0; lane < POLYPHONY - 1; lane++) {
		ASSERT_EQ(SQUARE, osc[lane].type);
	}
	ASSERT_EQ(SAW_DOWN, osc[POLYPHONY - 1].type);

	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	time_t t;
	srand((unsigned) time(&t));

	GREATEST_MAIN_BEGIN();

	SHUFFLE_TESTS(rand(), {
			RUN_TEST(init_oscillators_resets_all_lanes);
			RUN_TEST(set_oscillator_type_affects_only_given_lane);
		});

	GREATEST_MAIN_END();
}
