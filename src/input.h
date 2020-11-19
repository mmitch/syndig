#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

enum midi_event_type {
	NOTE_ON,
	NOTE_OFF,
	PROGRAM_CHANGE,
};

typedef struct {
	uint8_t note;
	uint8_t velocity;
} midi_data_note_on;

typedef struct {
	uint8_t note;
	uint8_t velocity;
} midi_data_note_off;

typedef struct {
	uint8_t program;
} midi_data_program_change;

typedef struct {
	enum midi_event_type type;
	union {
		midi_data_note_on        note_on;
		midi_data_note_off       note_off;
		midi_data_program_change program_change;
	} data;
} midi_event;

typedef int         (*midi_open_fn )(void);
typedef midi_event* (*midi_read_fn )(void);
typedef int         (*midi_close_fn)(void);

typedef struct {
	midi_open_fn  open;
	midi_read_fn  read;
	midi_close_fn close;
} midi_input;

midi_input* get_midi_input();

#endif
