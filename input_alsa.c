#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <alsa/asoundlib.h>

#include "input.h"

static snd_rawmidi_t* midi_in = NULL;

static uint8_t buffer[1024];
static const size_t buffer_size = sizeof(buffer);

static const midi_event NOTE_ON  = { .onoff = true  };
static const midi_event NOTE_OFF = { .onoff = false };

static int alsa_open()
{
	int err;
	
	if ((err = snd_rawmidi_open(&midi_in, NULL, "virtual", SND_RAWMIDI_NONBLOCK)) != 0) {
		fprintf(stderr, "alsa: could not open MIDI device: %s\n", snd_strerror(err));
		return 1;
	}

	return 0;
}

static midi_event* alsa_read()
{
	ssize_t status;

	// status combines both error value (when negative) and bytes read (zero or positive)
	// the ALSA API documentation does not describe the return value at all:
	// https://www.alsa-project.org/alsa-doc/alsa-lib/group___raw_midi.html#ga9b0952fc04facda83cdf1521325ca475
	status = snd_rawmidi_read(midi_in, &buffer, buffer_size);
	if (status < 0 && status != -EBUSY && status != -EAGAIN) {
		fprintf(stderr, "alsa: MIDI read error: %s\n", snd_strerror(status));
		return NULL;
	}

	if (status == 0) {
		// no data available
		return NULL;
	}
	
	// we only care about note on/note off (3 bytes)
	if (status == 3) {
		// FIXME: this simply assumes that a new MIDI packet is aligned with the buffer
		// FIXME: this listens on all MIDI channels
		uint8_t msb_nibble = buffer[0] & 0xF0;
		//  uint8_t lsb_nibble = buffer[0] & 0x0F; // LSB == MIDI Channel
		if (msb_nibble == 0x90) {
			return &NOTE_ON;
		}
		if (msb_nibble == 0x80) {
			return &NOTE_OFF;
		}
	}
	return NULL;
}

static int alsa_close()
{
	snd_rawmidi_close(midi_in);
	return 0;
}

midi_input alsa_input = {
	.open  = alsa_open,
	.read  = alsa_read,
	.close = alsa_close,
};
