#include <stdio.h>
#include <string.h>

#include "midi.h"
#include "oscillator.h"

int main() {
	sound_output *sound = NULL;
	sound = get_sound_output();
	if (sound->open() != 0) {
		goto SHUTDOWN;
	}

	midi_input *midi  = NULL;
	midi = get_midi_input();
	if (midi->open() != 0) {
		goto SHUTDOWN;
	}

	while(1) {
		receive_midi(midi);
		run_oscillator(sound);
	}

SHUTDOWN:
	if (midi != NULL) {
		midi->close();
	}

	if (sound != NULL) {
		sound->close();
	}

	return 0;
}
