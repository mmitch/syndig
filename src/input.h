#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

struct _midi_event {
	bool onoff;
	uint8_t note;
};

typedef struct _midi_event midi_event;

typedef int         (*midi_open_fn )(void);
typedef midi_event* (*midi_read_fn )(void);
typedef int         (*midi_close_fn)(void);

struct _midi_input {
	midi_open_fn  open;
	midi_read_fn  read;
	midi_close_fn close;
};

typedef const struct _midi_input midi_input;

midi_input* get_midi_input();

#endif
