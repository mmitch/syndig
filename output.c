#include "output.h"

extern sound_output pulse_output;

sound_output* get_sound_output() {
	return &pulse_output;
}
