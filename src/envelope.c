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
} state;

typedef struct {
	state state;
	float value;
	float velocity;
} envelope;

static envelope env[POLYPHONY];

void init_envelopes() {
	for (id id = 0; id < POLYPHONY; id++) {
		env[id].state    = OFF;
		env[id].value    = 0;
		env[id].velocity = 0;
	}
}

void trigger_envelope(id id, float new_velocity) {
	env[id].state    = ATTACK;
	env[id].velocity = new_velocity;
}

void release_envelope(id id) {
	env[id].state = RELEASE;
}

float envelope_nextval(id id) {

	envelope *e = &env[id];
	
	if (e->state == OFF) {
		return 0;
	}

	if (e->state == ATTACK) {
		if (attack == 0 || e->value >= e->velocity) {
			e->value = e->velocity;
			e->state = DECAY;
		} else {
			e->value = MIN( e->value + ((MAX_MIDI - attack * e->velocity) / (MAX_MIDI * 100)), e->velocity);
		}
	}
	if (e->state == DECAY) {
		if (decay == 0 || e->value <= sustain * e->velocity) {
			e->value = sustain * e->velocity;
			e->state = SUSTAIN;
		} else {
			e->value = MAX( e->value - ((MAX_MIDI - decay * e->velocity) / (MAX_MIDI * 100)), sustain * e->velocity);
		}
	}
	if (e->state == RELEASE) {
		if (release == 0 || e->value <= 0) {
			e->value = 0;
			e->state = OFF;
		} else {
			e->value = MAX( e->value - ((MAX_MIDI - release * e->velocity) / (MAX_MIDI * 100)), 0);
		}
	}

	return e->value;
}

bool envelope_is_running(id id) {
	return env[id].state != OFF;
}

bool  envelope_is_in_release(id id) {
	return env[id].state == RELEASE;
}
