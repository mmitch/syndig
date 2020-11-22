#ifndef _POLYPHONY_H_
#define _POLYPHONY_H_

#include "common.h"

typedef enum {
	KILL_OLDEST,
	KILL_LOWEST,
	KILL_HIGHEST,
	ROUND_ROBIN,
} polyphony_mode_t;

typedef struct {
	polyphony_mode_t mode;
	char*            name;
} polyphony_mode;

void init_polyphony();
void set_polyphony_mode(polyphony_mode new_mode);
void play_note(uint8_t note);
void stop_note(uint8_t note);

#endif
