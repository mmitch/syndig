#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <unistd.h>

#define BUFSIZE 128
#define BUFTYPE float
#define BUFBYTES (BUFSIZE * sizeof(BUFTYPE))
extern BUFTYPE samples[BUFSIZE];

#define SAMPLERATE 44100.0

typedef int     (*sound_open_fn )(void);
typedef ssize_t (*sound_write_fn)(const void *buf, size_t count);
typedef int     (*sound_close_fn)(void);

typedef const struct {
	sound_open_fn  open;
	sound_write_fn write;
	sound_close_fn close;
} sound_output;

void          clear_sample_buffer();
sound_output* get_sound_output();

#endif
