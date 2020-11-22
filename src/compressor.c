#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "output.h"

#define NEUTRAL 1.0
#define FALLOFF 0.01

static bool compressing = false;
static BUFTYPE compression = NEUTRAL;

void compress_buffer() {
	BUFTYPE min = 0;
	BUFTYPE max = 0;
	for (uint8_t i = 0; i < BUFSIZE; i++) {
		if (samples[i] < min) {
			min = samples[i];
		}
		if (samples[i] > max) {
			max = samples[i];
		}
	}
	max = MAX(max, -min);

	if (max > compression) {
		compression = max;
		if (! compressing) {
			printf("compressor kicked in at %f\n", compression);
			compressing = true;
		}
	} else {
		if (compression > NEUTRAL) {
			compression = MAX (compression - FALLOFF, NEUTRAL);
			if (compression == NEUTRAL) {
				compressing = false;
			}
		}
	}

	if (compressing) {
		for (uint8_t i = 0; i < BUFSIZE; i++) {
			samples[i] /= compressing;
		}
	}
}
