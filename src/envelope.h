#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

#define MAX_MIDI 128.0

void  trigger_envelope(float velocity);
void  release_envelope();
float envelope_nextval();

#endif
