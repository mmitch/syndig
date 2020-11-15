#include <stdint.h>

#include "input.h"
#include "output.h"

#define BUFSIZE 1024
static int16_t silence[BUFSIZE];
static int16_t sawtooth[BUFSIZE];


int main() {
	sound_output *sound = get_sound_output();
	midi_input *midi = get_midi_input();
	
	sound->open();
	midi->open();

	// init buffer with simple sawwave
	for (int i = 0; i < BUFSIZE; i++) {
		int16_t val = (i % 256) << 7;
		sawtooth[i] = val;
	}

	bool playing = false;
	while(true) {
		midi_event *input = midi->read();
		if (input != NULL) {
			playing = input->onoff;
		}
		if (playing) {
			sound->write(&sawtooth, BUFSIZE);
		} else {
			// FIXME: use usleep() instead of silent output
			sound->write(&silence, BUFSIZE);
		}
	}
	
	
	sound->close();
	midi->close();

	return 0;
}
