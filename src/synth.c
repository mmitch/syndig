#include <stdio.h>
#include <string.h>

#include "common.h"
#include "envelope.h"
#include "midi.h"
#include "oscillator.h"

int main() {
	printf("%s is starting\n", PROGRAM_NAME);

	sound_output *sound = NULL;
	sound = get_sound_output();
	if (sound->open() != 0) {
		goto SHUTDOWN;
	}
	puts("sound output opened");

	midi_input *midi  = NULL;
	midi = get_midi_input();
	if (midi->open() != 0) {
		goto SHUTDOWN;
	}
	puts("MIDI input opened");

	init_oscillator();
	puts("oscillator initialized");

	while(1) {
		receive_midi(midi);
		run_oscillator(sound);
	}

SHUTDOWN:
	if (midi != NULL) {
		midi->close();
		puts("MIDI input closed");
	}

	if (sound != NULL) {
		sound->close();
		puts("sound output closed");
	}

	return 0;
}
