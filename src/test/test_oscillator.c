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

#include "../channel.c" // contains no code, so no mock needed

static void setup() {
	init_oscillators();

	reset_envelope_mocks();
	FFF_RESET_HISTORY()
}

TEST init_oscillators_resets_all_oscillators() {
	// given
	setup();

	osc[2].frequency = 13;
	osc[1].wavelength = 17;

	// when
	init_oscillators();

	// then
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		ASSERT_EQ(440.0,  osc[lane].frequency);
		ASSERT_IN_RANGE(SAMPLERATE/440.0, osc[lane].wavelength, 0.0001);
	}

	PASS();
}

TEST init_oscillators_resets_oscillators_in_channel_configuration() {
	// given
	setup();

	ch_config[3].osc = SAW_DOWN;
	ch_config[7].osc = TRIANGLE;

	// when
	init_oscillators();

	// then
	for (channel_id channel = 0; channel < CHANNELS; channel++) {
		ASSERT_EQ(SQUARE,  ch_config[channel].osc);
	}

	PASS();
}

TEST set_oscillator_type_affects_only_given_channel() {
	// given
	setup();

	// when
	set_oscillator_type(CHANNELS - 1, SAW_DOWN);

	// then
	for (channel_id channel = 0; channel < CHANNELS - 1; channel++) {
		ASSERT_EQ(SQUARE, ch_config[channel].osc);
	}
	ASSERT_EQ(SAW_DOWN, ch_config[CHANNELS - 1].osc);

	PASS();
}

TEST run_oscillator_square_at_half_rate() {
	// given
	setup();

	lane_id    lane    = 3;
	channel_id channel = 7;
	set_oscillator_type(channel, SQUARE);
	set_oscillator_channel(lane, channel);
	set_oscillator_frequency(lane, SAMPLERATE / 2.0);

	// when
	BUFTYPE *buffer = run_oscillator(lane);

	// then
	for (uint8_t i = 0; i < BUFSIZE_MONO; i++) {
		float expected = i % 2 ? 1 : -1;
		ASSERT_EQ(expected, buffer[i]);
	}

	PASS();
}

#include "../debug.h"

TEST run_oscillator_wavelet_square_50_at_eigth_rate() {
	// given
	setup();

	lane_id    lane    = 3;
	channel_id channel = 7;
	set_oscillator_type(channel, WAVELET_SQUARE_50);
	set_oscillator_channel(lane, channel);
	set_oscillator_frequency(lane, SAMPLERATE / 8.0);

	// when
	BUFTYPE *buffer = run_oscillator(lane);

	// then
	for (uint8_t i = 0; i < BUFSIZE_MONO; i++) {
		float expected = wavelet_square_50[(i+1) % WAVELET_LENGTH];
		ASSERT_EQ(expected, buffer[i]);
	}

	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	time_t t;
	srand((unsigned) time(&t));

	GREATEST_MAIN_BEGIN();

	SHUFFLE_TESTS(rand(), {
			RUN_TEST(init_oscillators_resets_all_oscillators);
			RUN_TEST(init_oscillators_resets_oscillators_in_channel_configuration);
			RUN_TEST(set_oscillator_type_affects_only_given_channel);
			RUN_TEST(run_oscillator_square_at_half_rate);
			RUN_TEST(run_oscillator_wavelet_square_50_at_eigth_rate);
		});

	GREATEST_MAIN_END();
}
