#include "envelope.h"
#include "hertz.h"
#include "input.h"
#include "oscillator.h"

static oscillator_type program_map[256] = {
	SQUARE,
	SAW_DOWN,
	SAW_UP,
};

uint8_t note_playing = 255; // invalid note

void receive_midi(midi_input *midi) {
	midi_event *event;
	while ((event = midi->read()) != NULL) {
		switch (event->type) {

		case NOTE_ON:
			note_playing = event->data.note_on.note;
			set_oscillator_frequency(0, hertz[note_playing]);
			trigger_envelope(event->data.note_on.velocity / MAX_MIDI);
			break;

		case NOTE_OFF:
			if (event->data.note_on.note == note_playing) {
				release_envelope();
			}
			break;

		case PROGRAM_CHANGE:
			change_oscillator_type(program_map[event->data.program_change.program]);
		}
	}
}
