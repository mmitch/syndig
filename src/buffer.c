#include <string.h>

#include "buffer.h"

BUFTYPE samples[BUFSIZE];
BUFTYPE silence[BUFSIZE];

void init_sample_buffer() {
	for (int i = 0; i < BUFSIZE; i++) {
		silence[i] = 0;
	}
	clear_sample_buffer();
}

void clear_sample_buffer() {
	memcpy(samples, silence, BUFBYTES);
}
