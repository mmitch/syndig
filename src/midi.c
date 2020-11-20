#include "common.h"
#include "envelope.h"
#include "hertz.h"
#include "input.h"
#include "oscillator.h"
#include "polyphony.h"

#define LAST_INDEX(arr)          ((sizeof(arr) / (sizeof(arr[0]))) - 1)
#define CLAMP_TO_MAP(val, map)   { if (val > LAST_INDEX(map)) { val = 0; } }

#define to_struct(x) { .mode = x, .name = #x }
static polyphony_mode poly_map[] = {
	to_struct(KILL_OLDEST),
	to_struct(KILL_LOWEST),
	to_struct(KILL_HIGHEST),
	to_struct(ROUND_ROBIN),
};

static oscillator_type program_map[] = {
	SQUARE,
	SAW_DOWN,
	SAW_UP,
};

void receive_midi(midi_input *midi) {
	midi_event *event;
	while ((event = midi->read()) != NULL) {
		switch (event->type) {

		case NOTE_ON:
		{
			uint8_t note = event->data.note_on.note;
			lane_id lane = reserve_lane_for_note(note);
			set_oscillator_frequency(lane, hertz[note]);
			trigger_envelope(lane, event->data.note_on.velocity / MAX_MIDI);
			break;
		}

		case NOTE_OFF:
		{
			lane_id lane = find_lane_with_note(event->data.note_on.note);
			if (lane != NO_LANE) {
				release_envelope(lane);
			}
			break;
		}

		case PROGRAM_CHANGE:
		{
			uint8_t program = event->data.program_change.program;
			CLAMP_TO_MAP(program, program_map);
			change_oscillator_type(program_map[program]);
			break;
		}

		case CONTROL_CHANGE:
		{
			uint8_t value = event->data.control_change.value;
			switch (event->data.control_change.param) {
			case 3:
				CLAMP_TO_MAP(value, poly_map);
				set_polyphony_mode(poly_map[value]);
				break;
			}
			break;
		}
		}
	}
}
