#include <stdint.h>

#include "buffer.h"
#include "common.h"
#include "envelope.h"

#define MAX_MIDI  128.0
#define STRETCH   10

#define MS_TO_TICKS(x)  ((x) * SAMPLERATE / 1000.0)

static float  attack_rate;
static float  decay_rate;
static float  sustain_level;
static float  release_rate;

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
	for (lane_id lane = 0; lane < POLYPHONY; lane++) {
		env[lane].state    = OFF;
		env[lane].value    = 0;
		env[lane].velocity = 0;
	}
	set_envelope_attack(30);
	set_envelope_decay(50);
	sustain_level = 0.5;
	set_envelope_release(30);
}

void trigger_envelope(lane_id lane, uint8_t velocity) {
	env[lane].state    = ATTACK;
	env[lane].velocity = velocity / MAX_MIDI;
}

void release_envelope(lane_id lane) {
	env[lane].state = RELEASE;
}

float envelope_nextval(lane_id lane) {

	envelope *e = &env[lane];
	
	if (e->state == OFF) {
		return 0;
	}

	if (e->state == ATTACK) {
		if (attack_rate == 0 || e->value >= e->velocity) {
			e->value = e->velocity;
			e->state = DECAY;
		} else {
			e->value = MIN( e->value + (attack_rate * e->velocity), e->velocity);
		}
	}
	if (e->state == DECAY) {
		if (decay_rate == 0 || e->value <= sustain_level * e->velocity) {
			e->value = sustain_level * e->velocity;
			e->state = SUSTAIN;
		} else {
			e->value = MAX( e->value - (decay_rate  * e->velocity), sustain_level * e->velocity);
		}
	}
	if (e->state == RELEASE) {
		if (release_rate == 0 || e->value <= 0) {
			e->value = 0;
			e->state = OFF;
		} else {
			e->value = MAX( e->value - (release_rate * e->velocity), 0);
		}
	}

	return e->value;
}

bool envelope_is_running(lane_id lane) {
	return env[lane].state != OFF;
}

/*
 *           1
 *  rate = -----
 *         ticks
 */
void set_envelope_attack(uint8_t attack_ms) {
	if (attack_ms == 0) {
		attack_rate = 0;
	} else {
		attack_rate = 1.0 / ( MS_TO_TICKS(attack_ms) * STRETCH );
	}
}

void set_envelope_decay(uint8_t decay_ms) {
	if (decay_ms == 0) {
		decay_rate = 0;
	} else {
		decay_rate = 1.0 / ( MS_TO_TICKS(decay_ms) * STRETCH );
	}
}

void set_envelope_release(uint8_t release_ms) {
	if (release_ms == 0) {
		release_rate = 0;
	} else {
		release_rate = 1.0 / ( MS_TO_TICKS(release_ms) * STRETCH );
	}
}
