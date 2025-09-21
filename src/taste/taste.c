#include <taste/taste.h>
#include <taste/test.h>

#include "taste_exit.h"
#include "taste_serial.h"
#include "taste_state.h"

static bool run_test(TasteTest_t* test) {
	test->test(&test->context);
	bool test_passed = test->context.status == PASSED;

	taste_printf("\t%s%s: %s\n", test_passed ? "pass" : "FAIL",
	             (!test_passed && test->allowed_to_fail) ? " (allowed to fail)" : "", test->name);

	if (!test_passed && test->context.message != NULL) {
		taste_puts("\t\tinfo: ");
		taste_puts(test->context.message);
		taste_puts("\n");
	}

	return test_passed;
}

static bool run_test_suite(TasteTestSuite_t* test_suite) {
	taste_puts("TEST SUITE: ");
	taste_puts(test_suite->name);
	taste_puts("\n");

	long passed_tests = 0;
	long counted_tests = 0;
	for (long i = 0; i < test_suite->test_count; i++) {
		TasteTest_t* test = test_suite->tests[i]();
		bool test_result = run_test(test);
		if (test_result) {
			passed_tests++;
		}
		if (test_result || test->allowed_to_fail) {
			counted_tests++;
		}
	}

	bool test_suite_passed = counted_tests == test_suite->test_count;
	taste_printf("Test suite %s    (%ld / %ld)\n\n", test_suite_passed ? "passed" : "FAILED", passed_tests,
	             test_suite->test_count);

	return test_suite_passed;
}

void run_tests() {
	bool all_passed = true;
	taste_puts("Starting taste-tests...\n\n");
	for (long i = 0; i < test_suite_count; i++) {
		TasteTestSuite_t* test_suite = &test_suites[i];
		all_passed &= run_test_suite(test_suite);
	}

	taste_puts("Finished tests\n");
	taste_puts(all_passed ? TASTE_PASS_MESSAGE : TASTE_FAIL_MESSAGE);
	taste_puts("\n");

	taste_exit();
}
