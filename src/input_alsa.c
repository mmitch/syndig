/*
 * input_alsa.c - ALSA MIDI input driver
 *
 * Copyright (C) 2020  Christian Garbs <mitch@cgarbs.de>
 * Licensed under GNU GPL v3 (or later)
 *
 * This file is part of syndig, a simple software synthesizer written in C.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>

#include <alsa/asoundlib.h>

#include "input.h"

static snd_seq_t *sequencer;
static int port;

static midi_event event;

static int alsa_open()
{
	int err;
	
	if ((err = snd_seq_open(&sequencer, "default", SND_SEQ_OPEN_INPUT, SND_SEQ_NONBLOCK)) != 0) {
		fprintf(stderr, "alsa: could not open sequencer for input: %s\n", snd_strerror(err));
		return 1;
	}

	snd_seq_set_client_name(sequencer, PROGRAM_NAME);

	snd_seq_set_client_event_filter(sequencer, SND_SEQ_EVENT_NOTEON);
	snd_seq_set_client_event_filter(sequencer, SND_SEQ_EVENT_NOTEOFF);
	snd_seq_set_client_event_filter(sequencer, SND_SEQ_EVENT_PGMCHANGE);
	snd_seq_set_client_event_filter(sequencer, SND_SEQ_EVENT_CONTROLLER);

	port = snd_seq_create_simple_port(sequencer, "Input",
					  SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
					  SND_SEQ_PORT_TYPE_MIDI_GENERIC);
	if (port < 0) {
		fprintf(stderr, "alsa: could not open sequencer port: %s\n", snd_strerror(port));
		return 1;
	}

	snd_seq_addr_t destination;
	destination.client = snd_seq_client_id(sequencer);
	destination.port = port;

	snd_seq_port_subscribe_t *subscriptions;
	snd_seq_port_subscribe_alloca(&subscriptions);
	snd_seq_port_subscribe_set_dest(subscriptions, &destination);
	snd_seq_subscribe_port(sequencer, subscriptions);

	return 0;
}

static midi_event* alsa_read()
{
	int bytes_left;
	snd_seq_event_t *alsa_event;

	bytes_left = snd_seq_event_input(sequencer, &alsa_event);

	if (bytes_left == -EAGAIN) {
		return NULL;
	}

	if (bytes_left == -ENOSPC) {
		fprintf(stderr, "alsa: dropped some MIDI input events\n");
		return NULL;
	}

	// all types: https://www.alsa-project.org/alsa-doc/alsa-lib/group___seq_events.html#gaef39e1f267006faf7abc91c3cb32ea40
	switch (alsa_event->type) {

	case SND_SEQ_EVENT_NOTEON:
		event.type = NOTE_ON;
		event.channel               = alsa_event->data.note.channel;
		event.data.note_on.note     = alsa_event->data.note.note;
		event.data.note_on.velocity = alsa_event->data.note.velocity;
		return &event;

	case SND_SEQ_EVENT_NOTEOFF:
		event.type = NOTE_OFF;
		event.channel                = alsa_event->data.note.channel;
		event.data.note_off.note     = alsa_event->data.note.note;
		event.data.note_off.velocity = alsa_event->data.note.velocity;
		return &event;

	case SND_SEQ_EVENT_PGMCHANGE:
		event.type = PROGRAM_CHANGE;
		event.channel                     = alsa_event->data.control.channel;
		event.data.program_change.program = alsa_event->data.control.value;
		return &event;

	case SND_SEQ_EVENT_CONTROLLER:
		event.type = CONTROL_CHANGE;
		event.channel                   = alsa_event->data.control.channel;
		event.data.control_change.param = alsa_event->data.control.param;
		event.data.control_change.value = alsa_event->data.control.value;
		return &event;
	}

	return NULL;
}

static int alsa_close()
{
	int err;

	if ((err = snd_seq_delete_simple_port(sequencer, port)) != 0) {
		fprintf(stderr, "alsa: could not close sequencer port: %s\n", snd_strerror(err));
		return 0;
	}

	if ((err = snd_seq_close(sequencer)) != 0) {
		fprintf(stderr, "alsa: could not close sequencer: %s\n", snd_strerror(err));
		return 0;
	}

	return 0;
}

midi_input alsa_input = {
	.open  = alsa_open,
	.read  = alsa_read,
	.close = alsa_close,
};
