/*
 * test_midi.c - unit tests of midi.c
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

#include "../midi.c"

#include "../thirdparty/greatest.h"
#include "../thirdparty/fff.h"
DEFINE_FFF_GLOBALS

FAKE_VOID_FUNC(set_envelope_attack,  uint16_t)
FAKE_VOID_FUNC(set_envelope_release, uint16_t)
FAKE_VOID_FUNC(set_envelope_decay,   uint16_t)
FAKE_VOID_FUNC(set_envelope_sustain, float)

FAKE_VOID_FUNC(change_oscillator_type, oscillator_type)

FAKE_VOID_FUNC(play_note, uint8_t, float)
FAKE_VOID_FUNC(stop_note, uint8_t)
FAKE_VOID_FUNC(set_polyphony_mode, polyphony_mode)

FAKE_VALUE_FUNC0(midi_event *, read_midi)
midi_event* read_midi_proxy() {
	return read_midi();
}
midi_input midi = {
	.read = read_midi_proxy,
};

static midi_event event;

static midi_event* events[] = { NULL, NULL };

void setup() {
	RESET_FAKE(set_envelope_attack)
	RESET_FAKE(set_envelope_release)
	RESET_FAKE(set_envelope_decay)
	RESET_FAKE(set_envelope_sustain)
	RESET_FAKE(change_oscillator_type)
	RESET_FAKE(play_note)
	RESET_FAKE(stop_note)
	RESET_FAKE(set_polyphony_mode)
	RESET_FAKE(read_midi)
		
	FFF_RESET_HISTORY()
}

void send_fake_event(midi_event *e) {
	events[0] = e;
	SET_RETURN_SEQ(read_midi, events, 2)
}
	
TEST empty_event_does_nothing() {
	// given
	setup();

	send_fake_event(NULL);

	// when
	receive_midi(&midi);

	// then
	ASSERT_EQ(1, read_midi_fake.call_count);
	// TODO: really write "not called" for every possible function?
	PASS();
}

TEST note_on_is_passed_to_polyphony() {
	// given
	setup();

	event.type = NOTE_ON;
	event.data.note_on.note     = 37;
	event.data.note_on.velocity = 90;

	send_fake_event(&event);

	// when
	receive_midi(&midi);

	// then
	ASSERT_EQ(2,  read_midi_fake.call_count);

	ASSERT_EQ(1,  play_note_fake.call_count);
	ASSERT_EQ(37, play_note_fake.arg0_val);
	ASSERT_IN_RANGE(90 / MAX_MIDI, play_note_fake.arg1_val, 0.0001);

	PASS();
}

TEST note_off_is_passed_to_polyphony() {
	// given
	setup();

	event.type = NOTE_OFF;
	event.data.note_off.note = 76;

	send_fake_event(&event);

	// when
	receive_midi(&midi);

	// then
	ASSERT_EQ(2,  read_midi_fake.call_count);

	ASSERT_EQ(1,  stop_note_fake.call_count);
	ASSERT_EQ(76, stop_note_fake.arg0_val);

	PASS();
}

TEST program_change_is_passed_to_oscillator() {
	// given
	setup();

	event.type = PROGRAM_CHANGE;
	event.data.program_change.program = 2;

	send_fake_event(&event);

	// when
	receive_midi(&midi);

	// then
	ASSERT_EQ(2,      read_midi_fake.call_count);

	ASSERT_EQ(1,      change_oscillator_type_fake.call_count);
	ASSERT_EQ(SAW_UP, change_oscillator_type_fake.arg0_val);

	PASS();
}

TEST program_change_unmapped_values_map_to_square() {
	// given
	setup();

	event.type = PROGRAM_CHANGE;
	event.data.program_change.program = 99;

	send_fake_event(&event);

	// when
	receive_midi(&midi);

	// then
	ASSERT_EQ(2,      read_midi_fake.call_count);

	ASSERT_EQ(1,      change_oscillator_type_fake.call_count);
	ASSERT_EQ(SQUARE, change_oscillator_type_fake.arg0_val);

	PASS();
}

TEST controller_3_sets_polyphony_mode() {
	// given
	setup();

	event.type = CONTROL_CHANGE;
	event.data.control_change.param = 3;
	event.data.control_change.value = 3;

	send_fake_event(&event);

	// when
	receive_midi(&midi);

	// then
	ASSERT_EQ(2,           read_midi_fake.call_count);

	ASSERT_EQ(1,           set_polyphony_mode_fake.call_count);
	ASSERT_EQ(ROUND_ROBIN, set_polyphony_mode_fake.arg0_val.mode);

	PASS();
}

TEST controller_3_unmapped_values_map_to_kill_oldest() {
	// given
	setup();

	event.type = CONTROL_CHANGE;
	event.data.control_change.param = 3;
	event.data.control_change.value = 99;

	send_fake_event(&event);

	// when
	receive_midi(&midi);

	// then
	ASSERT_EQ(2,           read_midi_fake.call_count);

	ASSERT_EQ(1,           set_polyphony_mode_fake.call_count);
	ASSERT_EQ(KILL_OLDEST, set_polyphony_mode_fake.arg0_val.mode);

	PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	GREATEST_MAIN_BEGIN();

	RUN_TEST(empty_event_does_nothing);
	RUN_TEST(note_on_is_passed_to_polyphony);
	RUN_TEST(note_off_is_passed_to_polyphony);
	RUN_TEST(program_change_is_passed_to_oscillator);
	RUN_TEST(program_change_unmapped_values_map_to_square);
	RUN_TEST(controller_3_sets_polyphony_mode);
	RUN_TEST(controller_3_unmapped_values_map_to_kill_oldest);

	GREATEST_MAIN_END();
}
