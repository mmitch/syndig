#include <stdio.h>
#include <string.h>

#include "common.h"
#include "envelope.h"
#include "midi.h"
#include "oscillator.h"

int main() {
	sound_output *sound = NULL;
	midi_input *midi = NULL;

	printf("%s is starting\n", PROGRAM_NAME);

	sound = get_sound_output();
	if (sound->open() != 0) {
		goto SHUTDOWN;
	}
	puts("sound output opened");

	midi = get_midi_input();
	if (midi->open() != 0) {
		goto SHUTDOWN;
	}
	puts("MIDI input opened");

	init_midi();
	puts("MIDI mapper initialized");

	init_envelopes();
	puts("envelopes initialized");

	init_oscillators();
	puts("oscillator initialized");

	while(1) {
		receive_midi(midi);
		run_oscillators(sound);
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
