#ifndef _BUFFER_H_
#define _BUFFER_H_

#define BUFSIZE 128
#define BUFTYPE float
#define BUFBYTES (BUFSIZE * sizeof(BUFTYPE))
extern BUFTYPE samples[BUFSIZE];

#define SAMPLERATE 44100.0

void init_sample_buffer();
void clear_sample_buffer();

#endif
