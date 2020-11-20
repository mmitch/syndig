#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>

#define POLYPHONY 4

typedef uint8_t lane_id;
// max id + 1 is unknown/not found
#define NO_LANE POLYPHONY

extern const char *PROGRAM_NAME;

#endif
