#include <stdio.h>

#include "common.h"
#include "envelope.h"
#include "hertz.h"
#include "input.h"
#include "oscillator.h"

#define INVALID_NOTE 255
#define LOWEST_NOTE  0
#define HIGHEST_NOTE 127

static oscillator_type program_map[256] = {
	SQUARE,
	SAW_DOWN,
	SAW_UP,
};

typedef enum {
	ROUND_ROBIN,
	KILL_LOWEST,
	KILL_HIGHEST,
} polyphony_mode;

typedef struct {
	uint8_t note;
} lane_t;

static lane_t lane[POLYPHONY];

static polyphony_mode poly_mode = KILL_LOWEST;
static id round_robin;

static id find_lane_with_note(uint8_t note) {
	for (id id = 0; id < POLYPHONY; id++) {
		if (lane[id].note == note) {
			return id;
		}
	}
	return ID_NOT_FOUND;
}

static id find_free_lane() {
	for (id id = 0; id < POLYPHONY; id++) {
		if (! envelope_is_running(id)) {
			return id;
		}
	}

	for (id id = 0; id < POLYPHONY; id++) {
		if (envelope_is_in_release(id)) {
			return id;
		}
	}

	return ID_NOT_FOUND;
}

static id find_lane_for(uint8_t note) {

	id id, i;

	if ((id = find_lane_with_note(note)) != ID_NOT_FOUND) {
		return id;
	}

	if ((id = find_free_lane()) != ID_NOT_FOUND) {
		return id;
	}
	
	switch (poly_mode) {

	case ROUND_ROBIN:
		round_robin++;
		if (round_robin >= POLYPHONY) {
			round_robin = 0;
		}
		id = round_robin;
		break;

	case KILL_LOWEST:
		note = HIGHEST_NOTE;
		for (i = 0; i < POLYPHONY; i++) {
			if (lane[i].note <= note && lane[i].note != INVALID_NOTE) {
				id = i;
				note = lane[id].note;
			}
		}
		break;

	case KILL_HIGHEST:
		note = LOWEST_NOTE;
		for (i = 0; i < POLYPHONY; i++) {
			if (lane[i].note >= note && lane[i].note != INVALID_NOTE) {
				id = i;
				note = lane[id].note;
			}
		}
		break;
	}

	printf("polyphony overflow: killing note %d\n", lane[id].note);
	return id;
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
			uint8_t note = event->data.note_on.note;
			id id = find_lane_for(note);
			lane[id].note = note;
			set_oscillator_frequency(id, hertz[note]);
			trigger_envelope(id, event->data.note_on.velocity / MAX_MIDI);
			break;
		}

		case NOTE_OFF:
		{
			id id = find_lane_with_note(event->data.note_on.note);
			if (id != ID_NOT_FOUND) {
				release_envelope(id);
			}
			break;
		}

		case PROGRAM_CHANGE:
			change_oscillator_type(program_map[event->data.program_change.program]);
		}
	}
}
