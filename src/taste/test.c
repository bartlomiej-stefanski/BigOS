#include <taste/test.h>

#include "taste_state.h"

void register_test_suite(TasteTest_t* (**tests)(), long test_count, const char* test_suite) {
	test_suites[test_suite_count++] = (TasteTestSuite_t){.tests = tests, .test_count = test_count, .name = test_suite};
}
