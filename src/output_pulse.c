#include <stdio.h>
#include <unistd.h>

#include <pulse/error.h>
#include <pulse/simple.h>

#include "common.h"
#include "output.h"

static pa_simple *pulse_handle;

static int pulse_open()
{
	pa_sample_spec pulse_spec;
	pulse_spec.format = PA_SAMPLE_FLOAT32;
	pulse_spec.rate = 44100;
	pulse_spec.channels = 1;

	int err;
	pulse_handle = pa_simple_new(NULL, PROGRAM_NAME, PA_STREAM_PLAYBACK, NULL, PROGRAM_NAME, &pulse_spec, NULL, NULL, &err);
	if (!pulse_handle) {
		fprintf(stderr, "pulse: %s\n", pa_strerror(err));
		return -1;
	}

	return 0;
}

static ssize_t pulse_write(const void *buf, size_t count)
{
	return pa_simple_write(pulse_handle, buf, count, NULL);
}

static int pulse_close()
{
	pa_simple_free(pulse_handle);
	return 0;
}

sound_output pulse_output = {
	.open = pulse_open,
	.write = pulse_write,
	.close = pulse_close,
};
