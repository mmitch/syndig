#include "envelope.h"
#include "hertz.h"
#include "input.h"
#include "oscillator.h"

static enum oscillator_type program_map[256] = {
	SQUARE,
	SAW_DOWN,
	SAW_UP,
};

void receive_midi(midi_input *midi) {
	midi_event *event;
	while ((event = midi->read()) != NULL) {
		switch (event->type) {

		case NOTE_ON:
			set_oscillator_frequency(hertz[event->data.note_on.note]);
			trigger_envelope(event->data.note_on.velocity / MAX_MIDI);
			break;

		case NOTE_OFF:
			release_envelope();
			break;

		case PROGRAM_CHANGE:
			change_oscillator_type(program_map[event->data.program_change.program]);
		}
	}
}
