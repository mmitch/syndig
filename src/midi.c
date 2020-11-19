#include "envelope.h"
#include "hertz.h"
#include "input.h"
#include "oscillator.h"

void receive_midi(midi_input *midi) {
	midi_event *event;
	while ((event = midi->read()) != NULL) {
		if (event->onoff) {
			set_oscillator_frequency(hertz[event->note]);
			trigger_envelope(event->velocity / MAX_MIDI);
		} else {
			release_envelope();
		}
	}
}
