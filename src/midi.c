#include "envelope.h"
#include "hertz.h"
#include "input.h"
#include "oscillator.h"

extern oscillator_input oscillator;

void receive_midi(midi_input *midi) {
	midi_event *input;
	while ((input = midi->read()) != NULL) {
		if (input->onoff) {
			oscillator.frequency = hertz[input->note];
			trigger_envelope();
		} else {
			release_envelope();
		}
	}
}
