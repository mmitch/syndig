#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

#include <stdbool.h>

#include "common.h"

#define MAX_MIDI 128.0

void  init_envelopes();
void  trigger_envelope(lane_id id, float velocity);
void  release_envelope(lane_id id);
float envelope_nextval(lane_id id);
bool  envelope_is_running(lane_id id);
bool  envelope_is_in_release(lane_id id);

#endif
