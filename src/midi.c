#include "envelope.h"
#include "hertz.h"
#include "input.h"
#include "oscillator.h"

void receive_midi(midi_input *midi) {
	midi_event *input;
	while ((input = midi->read()) != NULL) {
		if (input->onoff) {
			set_oscillator_frequency(hertz[input->note]);
			trigger_envelope();
		} else {
			release_envelope();
		}
	}
}
