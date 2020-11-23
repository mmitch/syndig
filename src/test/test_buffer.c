#include "../buffer.c"

#include "stdio.h"
#define START_TESTS         { puts(__FILE__ ": start tests"); }
#define RUN_TEST(testcase)  { char *result = testcase(); if (result == NULL) { puts("  " #testcase " ok "); } else { puts( "  ERROR IN " #testcase ":" ); puts(result); return -1; }}
#define ASSERT(condition)   { if (! (condition)) { return "ASSERTION FAILED: " #condition; } }
#define END_TESTS           { puts(__FILE__ ": all tests ok"); return 0; }

static char* init_sample_buffer_sets_buffer_to_zero() {
	// given

	// when
	init_sample_buffer();

	// when
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

	// when
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
