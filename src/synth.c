#include <stdio.h>

#include "buffer.h"
#include "common.h"
#include "compressor.h"
#include "midi.h"
#include "oscillator.h"
#include "output.h"
#include "polyphony.h"

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

	init_sample_buffer();
	puts("buffer initialized");

	init_polyphony();
	puts("polyphony initialized");

	init_envelopes();
	puts("envelopes initialized");

	init_oscillators();
	puts("oscillator initialized");

	while(1) {
		receive_midi(midi);
		clear_sample_buffer();
		run_oscillators();
		compress_buffer();
		sound->write();
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
