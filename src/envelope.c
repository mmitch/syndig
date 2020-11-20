#include <stdint.h>

#include "envelope.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

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

static states state    = OFF;
static float  value    = 0;
static float  velocity = 0;

void trigger_envelope(float new_velocity) {
	state    = ATTACK;
	velocity = new_velocity;
}

void release_envelope() {
	state = RELEASE;
}

float envelope_nextval() {
	if (state == OFF) {
		return 0;
	}

	if (state == ATTACK) {
		if (attack == 0 || value >= velocity) {
			value = velocity;
			state = DECAY;
		} else {
			value = MIN( value + ((MAX_MIDI - attack * velocity) / (MAX_MIDI * 100)), velocity);
		}
	}
	if (state == DECAY) {
		if (decay == 0 || value <= sustain * velocity) {
			value = sustain * velocity;
			state = SUSTAIN;
		} else {
			value = MAX( value - ((MAX_MIDI - decay * velocity) / (MAX_MIDI * 100)), sustain * velocity);
		}
	}
	if (state == RELEASE) {
		if (release == 0 || value <= 0) {
			value = 0;
			state = OFF;
		} else {
			value = MAX( value - ((MAX_MIDI - release * velocity) / (MAX_MIDI * 100)), 0);
		}
	}

	return value;
}

bool envelope_is_running() {
	return state != OFF;
}
