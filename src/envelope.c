#include <stdint.h>

#include "envelope.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define MAX_MIDI 128.0

static uint8_t  attack  = 30;
static uint8_t  decay   = 50;
static float    sustain = 0.3;
static uint8_t  release = 127;

typedef enum {
	ATTACK,
	DECAY,
	SUSTAIN,
	RELEASE,
	OFF
} states;

static states state = OFF;
static float  value = 0;

void trigger_envelope() {
	state = ATTACK;
}

void release_envelope() {
	state = RELEASE;
}

float envelope_nextval() {
	if (state == OFF) {
		return 0;
	}

	if (state == ATTACK) {
		if (attack == 0 || value >= 1) {
			value = 1;
			state = DECAY;
		} else {
			value = MIN( value + ((MAX_MIDI - attack) / (MAX_MIDI * 100)), 1);
		}
	}
	if (state == DECAY) {
		if (decay == 0 || value <= sustain) {
			value = sustain;
			state = SUSTAIN;
		} else {
			value = MAX( value - ((MAX_MIDI - decay) / (MAX_MIDI * 100)), sustain);
		}
	}
	if (state == RELEASE) {
		if (release == 0 || value <= 0) {
			value = 0;
			state = OFF;
		} else {
			value = MAX( value - ((MAX_MIDI - release) / (MAX_MIDI * 100)), 0);
		}
	}

	return value;
}
