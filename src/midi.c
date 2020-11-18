#include "input.h"
#include "hertz.h"
#include "oscillator.h"

extern oscillator_input oscillator;

void receive_midi(midi_input *midi) {
	midi_event *input;
	while ((input = midi->read()) != NULL) {
		oscillator.active    = input->onoff;
		oscillator.frequency = hertz[input->note];
	}
}
