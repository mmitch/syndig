/*
 * test_envelope.c - unit test of envelope.c
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

#include "../envelope.c"

#include "../thirdparty/greatest.h"

#include "../channel.c" // contains no code, so no mock needed

#define RATE(x) ( 1.0 / MS_TO_TICKS((x)) )
#define ASSERT_RATE_EQ(x, y) ASSERT_IN_RANGE( RATE(x), (y), 0.0001 )

static void setup(void) {
	init_envelopes();
}

TEST init_envelopes_resets_lane_envelopes(void) {
	// given
	setup();

	env[3].state   = RELEASE;
	env[7].value   = 0.2;
	env[4].channel = 1;

	// when
	init_envelopes();

	// then
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		ASSERT_EQ(OFF, env[lane].state);
		ASSERT_EQ(  0, env[lane].value);
		ASSERT_EQ(  0, env[lane].channel);
	}

	PASS();
}

TEST init_envelopes_resets_channel_envelopes(void) {
	// given
	setup();

	ch_config[3].env.attack_rate   = 0;
	ch_config[4].env.decay_rate    = 0;
	ch_config[5].env.sustain_level = 0;
	ch_config[6].env.release_rate  = 0;

	// when
	init_envelopes();

	// then
	for (channel_id channel = 0; channel < CHANNELS; channel++) {
		ASSERT_RATE_EQ( 30, ch_config[channel].env.attack_rate);
		ASSERT_RATE_EQ( 50, ch_config[channel].env.decay_rate);
		ASSERT_EQ(0.5, ch_config[channel].env.sustain_level);
		ASSERT_RATE_EQ( 30, ch_config[channel].env.release_rate);
	}

	PASS();
}

TEST trigger_envelope_sets_channel_and_state(void) {
	// given
	setup();

	lane_id    lane = 7;
	channel_id channel = 3;

	// when
	trigger_envelope(lane, channel);

	// then
	ASSERT_EQ(ATTACK,  env[lane].state);
	ASSERT_EQ(channel, env[lane].channel);

	PASS();
}

TEST release_envelope_sets_state(void) {
	// given
	setup();

	lane_id lane = 7;

	// when
	release_envelope(lane);

	// then
	ASSERT_EQ(RELEASE, env[lane].state);

	PASS();
}

TEST stop_envelope_sets_state(void) {
	// given
	setup();

	lane_id lane = 7;

	env[lane].state = DECAY;

	// when
	stop_envelope(lane);

	// then
	ASSERT_EQ(OFF, env[lane].state);

	PASS();
}

TEST envelope_is_running_checks_state(void) {
	// given
	setup();

	env[0].state = ATTACK;
	env[1].state = DECAY;
	env[2].state = SUSTAIN;
	env[3].state = RELEASE;
	env[4].state = OFF;

	// when/then
	ASSERT_EQ( true, envelope_is_running(0));
	ASSERT_EQ( true, envelope_is_running(1));
	ASSERT_EQ( true, envelope_is_running(2));
	ASSERT_EQ( true, envelope_is_running(3));
	ASSERT_EQ(false, envelope_is_running(4));

	PASS();
}


TEST set_envelope_sustain_updates_channel_sustain(void) {
	// given
	setup();

	ch_config[5].env.sustain_level = 0.2;

	// when
	set_envelope_sustain(5, 0.75);

	// then
	ASSERT_EQ(0.75, ch_config[5].env.sustain_level);

	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	time_t t;
	srand((unsigned) time(&t));

	GREATEST_MAIN_BEGIN();

	SHUFFLE_TESTS(rand(), {
			RUN_TEST(init_envelopes_resets_lane_envelopes);
			RUN_TEST(init_envelopes_resets_channel_envelopes);
			RUN_TEST(trigger_envelope_sets_channel_and_state);
			RUN_TEST(release_envelope_sets_state);
			RUN_TEST(stop_envelope_sets_state);
			RUN_TEST(envelope_is_running_checks_state);
			RUN_TEST(set_envelope_sustain_updates_channel_sustain);
		});

	GREATEST_MAIN_END();
}
