#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <unistd.h>

typedef int     (*sound_open_fn )(void);
typedef ssize_t (*sound_write_fn)(const void *buf, size_t count);
typedef int     (*sound_close_fn)(void);

struct _sound_output {
	sound_open_fn  open;
	sound_write_fn write;
	sound_close_fn close;
};

typedef const struct _sound_output sound_output;

sound_output* get_sound_output();

#endif
