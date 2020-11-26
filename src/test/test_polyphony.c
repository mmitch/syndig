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

void setup() {
	reset_envelope_mocks();
	reset_oscillator_mocks();
		
	FFF_RESET_HISTORY()
}

TEST init_polyphony_resets_last_notes() {
	// given

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

	// when
	init_polyphony();

	// then
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		ASSERT_EQ(lane, poly_history[lane]);
	}
	
	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	time_t t;
	srand((unsigned) time(&t));

	GREATEST_MAIN_BEGIN();

	SHUFFLE_TESTS(rand(), {
			RUN_TEST(init_polyphony_resets_last_notes);
			RUN_TEST(init_polyphony_resets_poly_history);
		});

	GREATEST_MAIN_END();
}
