/*
 * test_hertz.c - unit test of hertz.c
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

#include "../hertz.c"

#include "../thirdparty/greatest.h"

#define TOLERANCE 0.00000000001

TEST test_some_notes(void) {
	ASSERT_IN_RANGE(  32.7031956626, hertz[ 24], TOLERANCE);
	ASSERT_IN_RANGE( 110.0,          hertz[ 45], TOLERANCE);
	ASSERT_IN_RANGE( 440.0,          hertz[ 69], TOLERANCE);
	ASSERT_IN_RANGE(8372.0180896192, hertz[120], TOLERANCE);

	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	time_t t;
	srand((unsigned) time(&t));

	GREATEST_MAIN_BEGIN();

	SHUFFLE_TESTS(rand(), {
			RUN_TEST(test_some_notes);
		});

	GREATEST_MAIN_END();
}
