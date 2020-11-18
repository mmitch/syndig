#include <stdint.h>

#include "envelope.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define MAX_MIDI 128.0

static uint8_t  attack  = 127;
static uint8_t  decay   = 30;
static float    sustain = 1;
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

static void adsr_trigger() {
	state = ATTACK;
}

static void adsr_release() {
	state = RELEASE;
}

static float adsr_nextval() {
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

envelope adsr_envelope = {
	.trigger = adsr_trigger,
	.release = adsr_release,
	.nextval = adsr_nextval,
};
