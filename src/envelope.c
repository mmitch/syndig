#include "envelope.h"

extern envelope adsr_envelope;

envelope* get_envelope() {
	return &adsr_envelope;
}
