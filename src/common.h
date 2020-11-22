#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>

#define POLYPHONY 16

typedef uint8_t lane_id;
// max id + 1 is unknown/not found
#define NO_LANE POLYPHONY

#define PROGRAM_NAME "synth"

// sys/param.h has these too and gets included by PulseAudio
#ifndef MIN
#define MIN(x, y)  ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y)  ((x) > (y) ? (x) : (y))
#endif

#endif
