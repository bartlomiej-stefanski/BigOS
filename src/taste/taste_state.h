#ifndef _TASTE_STATE_
#define _TASTE_STATE_

#include <taste/test.h>

#define MAX_TEST_SUITES 100

typedef struct TasteTestSuite_t {
	TasteTest_t* (**tests)();
	long test_count;
	const char* name;
} TasteTestSuite_t;

extern TasteTestSuite_t test_suites[MAX_TEST_SUITES];
extern long test_suite_count;

#endif
