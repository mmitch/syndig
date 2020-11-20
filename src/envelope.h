#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

#include <stdbool.h>

#include "common.h"

#define MAX_MIDI 128.0

void  init_envelopes();
void  trigger_envelope(id id, float velocity);
void  release_envelope(id id);
float envelope_nextval(id id);
bool  envelope_is_running(id id);

#endif
