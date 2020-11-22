#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

#include <stdbool.h>

#include "common.h"

void  init_envelopes();
void  set_envelope_attack(uint8_t new_attack);
void  set_envelope_decay(uint8_t new_decay);
void  set_envelope_release(uint8_t new_release);
void  trigger_envelope(lane_id id, uint8_t velocity);
void  release_envelope(lane_id id);
float envelope_nextval(lane_id id);
bool  envelope_is_running(lane_id id);

#endif
