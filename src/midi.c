#include <math.h>

#include "common.h"
#include "envelope.h"
#include "input.h"
#include "oscillator.h"
#include "polyphony.h"

#define MAX_MIDI  128.0

#define LAST_INDEX(arr)          ((sizeof(arr) / (sizeof(arr[0]))) - 1)
#define CLAMP_TO_MAP(val, map)   { if (val > LAST_INDEX(map)) { val = 0; } }

/*
 * stretch MIDI range [0..127] into bigger envelope range [0ms..~5000ms]
 * this function is roughly linear from 0->0 to about 64->900
 * and then goes on more exponentially to about 127->5000
 * gnuplot> plot [0:127] x**1.62,1.064**x,x**1.62+1.064**x
 */
#define STRETCH(x)  (powf((x), 1.62) + powf( 1.064, (x)))

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
			play_note(event->data.note_on.note, event->data.note_on.velocity / MAX_MIDI);
			break;
		}

		case NOTE_OFF:
		{
			stop_note(event->data.note_on.note);
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

			case 72:
				set_envelope_release(STRETCH(value));
				break;
				
			case 73:
				set_envelope_attack(STRETCH(value));
				break;
				
			case 75:
				set_envelope_sustain(value / MAX_MIDI);
				break;

			case 80:
				set_envelope_decay(STRETCH(value));
				break;
				
			}
			break;
		}
		}
	}
}
