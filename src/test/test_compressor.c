/*
 * test_compressor.c - unit tests of compressor.c
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

#include "../compressor.c"

#include "../thirdparty/greatest.h"

BUFTYPE stereo_out[BUFSIZE_STEREO];

static void reset_compressor() {
	compressing = false;
	compression = NEUTRAL;
	for (uint16_t i = 0; i < BUFSIZE_STEREO; i++) {
		stereo_out[i] = 0;
	}
}

TEST compression_of_empty_buffer_does_nothing() {
	// given
	reset_compressor();

	// when
	compress_buffer();

	// then
	for (uint16_t i = 0; i < BUFSIZE_STEREO; i++) {
		ASSERT_EQ(0.0, stereo_out[i]);
	}

	ASSERT_EQ(NEUTRAL, compression);
	ASSERT_EQ(false,   compressing);

	PASS();
}

TEST compression_does_not_kick_in_with_max_values() {
	// given
	reset_compressor();

	stereo_out[0] =  1.0;
	stereo_out[1] = -1.0;

	// when
	compress_buffer();

	// then
	ASSERT_EQ( 1.0, stereo_out[0]);
	ASSERT_EQ(-1.0, stereo_out[1]);
	ASSERT_EQ( 0.0, stereo_out[2]);

	ASSERT_EQ(NEUTRAL, compression);
	ASSERT_EQ(false,   compressing);

	PASS();
}

TEST compression_kicks_in_with_overflown_values() {
	// given
	reset_compressor();

	stereo_out[0] =  2.0;
	stereo_out[1] = -1.0;

	// when
	compress_buffer();

	// then
	ASSERT_EQ( 1.0, stereo_out[0]);
	ASSERT_EQ(-0.5, stereo_out[1]);
	ASSERT_EQ( 0.0, stereo_out[2]);

	ASSERT_EQ(2.0,  compression);
	ASSERT_EQ(true, compressing);

	PASS();
}

TEST compression_normalizes_the_whole_buffer() {
	// given
	reset_compressor();
	for (uint16_t i = 0; i < BUFSIZE_STEREO; i++) {
		stereo_out[i] = i;
	}

	// when
	compress_buffer();

	// then
	for (uint16_t i = 0; i < BUFSIZE_STEREO; i++) {
		ASSERT(stereo_out[i] <= 1.0);
	}

	PASS();
}

TEST compression_continues_on_next_call_but_reduced() {
	// given
	reset_compressor();

	stereo_out[0] =  2.0;
	stereo_out[1] =  1.0;
	compress_buffer();

	stereo_out[0] =  1.0;
	stereo_out[1] = -1.0;

	// when
	compress_buffer();

	// then
	const float expected_compression = (2.0 - FALLOFF);       // == 1.99
	const float expected_sample = 1.0 / expected_compression; // =~ 0.502513
	ASSERT_EQ( expected_sample, stereo_out[0]);
	ASSERT_EQ(-expected_sample, stereo_out[1]);

	ASSERT_EQ(expected_compression, compression);
	ASSERT_EQ(true,                 compressing);

	PASS();
}

TEST active_compression_can_raise_compression_level_if_needed() {
	// given
	reset_compressor();

	stereo_out[0] =  2.0;
	stereo_out[1] =  1.0;
	compress_buffer();

	stereo_out[0] =  4.0;
	stereo_out[1] = -1.0;

	// when
	compress_buffer();

	// then
	ASSERT_EQ( 1.0,  stereo_out[0]);
	ASSERT_EQ(-0.25, stereo_out[1]);

	ASSERT_EQ(4.0,  compression);
	ASSERT_EQ(true, compressing);

	PASS();
}

TEST compression_can_run_out() {
	// given
	reset_compressor();

	stereo_out[0] =  1.02;
	compress_buffer();
	compress_buffer();
	
	stereo_out[0] =  1.0;

	// when
	compress_buffer();

	// then
	ASSERT_EQ(1.0, stereo_out[0]);

	ASSERT_EQ(NEUTRAL, compression);
	ASSERT_EQ(false,   compressing);

	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	time_t t;
	srand((unsigned) time(&t));

	GREATEST_MAIN_BEGIN();

	SHUFFLE_TESTS(rand(), {
			RUN_TEST(compression_of_empty_buffer_does_nothing);
			RUN_TEST(compression_does_not_kick_in_with_max_values);
			RUN_TEST(compression_kicks_in_with_overflown_values);
			RUN_TEST(compression_normalizes_the_whole_buffer);
			RUN_TEST(compression_continues_on_next_call_but_reduced);
			RUN_TEST(active_compression_can_raise_compression_level_if_needed);
			RUN_TEST(compression_can_run_out);
		});

	GREATEST_MAIN_END();
}
