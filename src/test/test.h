#include "stdio.h"
#define START_TESTS         { puts(__FILE__ ": start tests"); }
#define RUN_TEST(testcase)  { char *result = testcase(); if (result == NULL) { puts("  " #testcase " ok "); } else { puts( "  ERROR IN " #testcase ":" ); puts(result); return -1; }}
#define ASSERT(condition)   { if (! (condition)) { return "ASSERTION FAILED: " #condition; } }
#define END_TESTS           { puts(__FILE__ ": all tests ok"); return 0; }
