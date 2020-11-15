#include <inttypes.h>

#include "output.h"

#define BUFSIZE 4096
static int16_t buffer[BUFSIZE];

// TODO: why doesn't this work?
// static const size_t BUFSIZE = 4096;
// static int16_t buffer[BUFSIZE];

int main() {
	sound_output* sound = get_sound_output();
	sound->open();

	// init buffer with simple sawwave
	for (int i = 0; i < BUFSIZE; i++) {
		int16_t val = (i % 256) << 7;
		buffer[i] = val;
	}

	// output buffer multiple times
	for (int repeat = 0; repeat < 64; repeat++) {
		sound->write(&buffer, BUFSIZE);
	}
	
	sound->close();
	return 0;
}
