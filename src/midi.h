#ifndef _MIDI_H_
#define _MIDI_H_

#include "envelope.h"
#include "input.h"

void init_midi();
void receive_midi(midi_input *midi);

#endif
