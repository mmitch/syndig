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

extern void    init_polyphony();
extern void    set_polyphony_mode(polyphony_mode new_mode);
extern lane_id find_lane_with_note(uint8_t note);
extern lane_id reserve_lane_for_note(uint8_t note);

#endif
