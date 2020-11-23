#include "../buffer.c"

#include "test.h"

static char* init_sample_buffer_sets_buffer_to_zero() {
	// given

	// when
	init_sample_buffer();

	// then
	for (int i=0; i<BUFSIZE; i++) {
		ASSERT(samples[i] == 0.0);
	}

	return NULL;
}

static char* clear_sample_buffer_sets_buffer_to_zero() {
	// given
	init_sample_buffer();
	samples[0] = 1337;

	// when
	clear_sample_buffer();

	// then
	for (int i=0; i<BUFSIZE; i++) {
		ASSERT(samples[i] == 0.0);
	}

	return NULL;
}

int main() {
	START_TESTS;
	RUN_TEST(init_sample_buffer_sets_buffer_to_zero);
	RUN_TEST(clear_sample_buffer_sets_buffer_to_zero);
	END_TESTS;
}
