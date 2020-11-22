#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

#include <stdbool.h>

#include "common.h"

void  init_envelopes();
void  set_envelope_attack(uint16_t attack_ms);
void  set_envelope_decay(uint16_t decay_ms);
void  set_envelope_release(uint16_t release_ms);
void  trigger_envelope(lane_id id, uint8_t velocity);
void  release_envelope(lane_id id);
float envelope_nextval(lane_id id);
bool  envelope_is_running(lane_id id);

#endif
