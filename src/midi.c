#include "common.h"
#include "envelope.h"
#include "hertz.h"
#include "input.h"
#include "oscillator.h"

#define INVALID_NOTE 255

static oscillator_type program_map[256] = {
	SQUARE,
	SAW_DOWN,
	SAW_UP,
};

typedef struct {
	uint8_t note;
} lane_t;

static lane_t lane[POLYPHONY];

static id current_lane;

static id find_lane_with_note(uint8_t note) {
	for (id id = 0; id < POLYPHONY; id++) {
		if (lane[id].note == note) {
			return id;
		}
	}
	return ID_NOT_FOUND;
}

static id get_free_lane() {
	id id;
	if ((id = find_lane_with_note(INVALID_NOTE)) != ID_NOT_FOUND) {
		return id;
	}

	current_lane++;
	if (current_lane >= POLYPHONY) {
		current_lane = 0;
	}
	return current_lane;
}

void init_midi() {
	for (id id = 0; id < POLYPHONY; id++) {
		lane[id].note = INVALID_NOTE;
	}
}

void receive_midi(midi_input *midi) {
	midi_event *event;
	while ((event = midi->read()) != NULL) {
		switch (event->type) {

		case NOTE_ON:
		{
			id id = get_free_lane();
			lane[id].note = event->data.note_on.note;
			set_oscillator_frequency(id, hertz[event->data.note_on.note]);
			trigger_envelope(id, event->data.note_on.velocity / MAX_MIDI);
			break;
		}

		case NOTE_OFF:
		{
			id id = find_lane_with_note(event->data.note_on.note);
			if (id != ID_NOT_FOUND) {
				release_envelope(id);
				lane[id].note = INVALID_NOTE;
			}
			break;
		}

		case PROGRAM_CHANGE:
			change_oscillator_type(program_map[event->data.program_change.program]);
		}
	}
}
