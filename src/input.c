#include "input.h"

extern midi_input alsa_input;

midi_input* get_midi_input() {
	return &alsa_input;
}
