/*
 * test_buffer.c - unit test of buffer.c
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

#include "../buffer.c"

#include "../thirdparty/greatest.h"

TEST init_sample_buffer_sets_buffer_to_zero() {
	// given

	// when
	init_sample_buffer();

	// then
	for (int i=0; i<BUFSIZE; i++) {
		ASSERT_EQ(samples[i], 0.0);
	}

	PASS();
}

TEST clear_sample_buffer_sets_buffer_to_zero() {
	// given
	init_sample_buffer();
	samples[0] = 1337;

	// when
	clear_sample_buffer();

	// then
	for (int i=0; i<BUFSIZE; i++) {
		ASSERT_EQ(samples[i], 0.0);
	}

	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	GREATEST_MAIN_BEGIN();

	RUN_TEST(init_sample_buffer_sets_buffer_to_zero);
	RUN_TEST(clear_sample_buffer_sets_buffer_to_zero);

	GREATEST_MAIN_END();
}
