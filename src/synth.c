#include <stdint.h>

#include "hertz.h"
#include "input.h"
#include "output.h"

#define BUFSIZE 128
#define BUFTYPE uint16_t
#define BUFBYTES ( BUFSIZE * sizeof( BUFTYPE ) )
static BUFTYPE silence[BUFSIZE];
static BUFTYPE square[BUFSIZE];

int main() {
	sound_output *sound = get_sound_output();
	midi_input *midi = get_midi_input();
	
	sound->open();
	midi->open();

	bool playing = false;
	uint16_t wavelength;
	uint16_t wavelength_half;
	uint16_t phase = 0;
	BUFTYPE value = 1;
	while(true) {
		midi_event *input = midi->read();
		if (input != NULL) {
			playing = input->onoff;
			if (playing) {
				// FIXME: get samplerate from somehwere else
				wavelength = 44100.0 / hertz[input->note];
				wavelength_half = wavelength / 2;
			}
		}
		if (playing) {
			for (int i = 0; i < BUFSIZE; i++) {
				phase++;
				if (phase >= wavelength) {
					phase = 0;
					value = 1;
				}
				if (phase == wavelength_half) {
					value = 1 << 14;
				}
				square[i]   = value;
			}
			sound->write(&square, BUFBYTES);
		} else {
			// FIXME: use usleep() instead of silent output
			sound->write(&silence, BUFBYTES);
		}
	}
	
	
	sound->close();
	midi->close();

	return 0;
}
