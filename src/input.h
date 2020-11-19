#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

typedef struct {
	bool onoff;
	uint8_t note;
	uint8_t velocity;
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
