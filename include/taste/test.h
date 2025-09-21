#ifndef TASTE_TESTS
	#error "Test setup must be enabled to use taste-tests"
#endif

#ifndef _TASTE_TEST_
	#define _TASTE_TEST_

	#include <stddef.h>

typedef struct TestContext_t {
	enum TestStatus {
		FAILED,
		PASSED
	} status;
	const char* message;
} TestContext_t;

typedef struct TasteTest_t {
	void (*test)(TestContext_t*);
	const char* name;
	bool allowed_to_fail;
	TestContext_t context;
} TasteTest_t;

void register_test_suite(TasteTest_t* (**tests)(), long test_count, const char* test_suite);

	#define TASTE_TEST_NAME(test_name) _taste_test_##test_name

	#define TASTE_TEST_STRUCT(test_name) _taste_test_struct##test_name

	#define TASTE_TEST_SUITE_NAME(test_suite) _taste_test_suite_##test_suite

	#define TASTE_TEST_SUITE_REG_NAME(test_suite) _taste_test_suite_reg_##test_suite

    /// If the test_name in TEST_SUITE is marked as ALLOWED_TO_FAIL then the test
    /// failing will not fail the test suite.
	#define ALLOWED_TO_FAIL(test_name) _allow_fail_##test_name

    /// Craete a test. Use of this macro should be followed by a function definition
    /// where the testing will be performed.
    /// ASSERT_* macros can be used in the test.
	#define TEST(test_name)                                                                \
		/* Forward definition of test function */                                          \
		static void TASTE_TEST_NAME(test_name)(TestContext_t * __context);                 \
		/* Static struct with information about the test */                                \
		static TasteTest_t TASTE_TEST_STRUCT(test_name) = {                                \
		    .test = TASTE_TEST_NAME(test_name),                                            \
		    .name = #test_name,                                                            \
		    .allowed_to_fail = false,                                                      \
		    .context =                                                                     \
		        (TestContext_t){                                                           \
		                        .status = PASSED,                                                      \
		                        .message = 0,                                                          \
		                        },                                                                         \
		};                                                                                 \
		/* Functions used for registering test in test-suite */                            \
		static TasteTest_t* test_name() {                                                  \
			return &TASTE_TEST_STRUCT(test_name);                                          \
		}                                                                                  \
		[[maybe_unused]] static TasteTest_t* ALLOWED_TO_FAIL(test_name)() {                \
			TASTE_TEST_STRUCT(test_name).allowed_to_fail = true;                           \
			return test_name();                                                            \
		}                                                                                  \
		/* Definition of testing function, user supplies implementation */                 \
		static void TASTE_TEST_NAME(test_name)([[maybe_unused]] TestContext_t * __context)

    /// Assert value is equal to expected.
    /// On success returns value.
    /// On failiure exits from the test.
	#define ASSERT_EQ(value, expected, info)                                   \
		({                                                                     \
			if ((value) != (expected)) {                                       \
				__context->status = FAILED;                                    \
				__context->message = "(" #value " != " #expected ") -> " info; \
				return;                                                        \
			}                                                                  \
			(value);                                                           \
		})

    /// Assert value is equal to expected.
    /// On success returns value.
    /// On failiure exits from the test.
	#define ASSERT_NEQ(value, expected, info)                                  \
		({                                                                     \
			if ((value) == (expected)) {                                       \
				__context->status = FAILED;                                    \
				__context->message = "(" #value " == " #expected ") -> " info; \
				return;                                                        \
			}                                                                  \
			(value);                                                           \
		})

    /// Assert value is true.
    /// On success returns value.
    /// On failiure exits from the test.
	#define ASSERT_TRUE(value, info)                                              \
		({                                                                        \
			if (!(value)) {                                                       \
				__context->status = FAILED;                                       \
				__context->message = "expected (" #value ") to be true -> " info; \
				return;                                                           \
			}                                                                     \
			true;                                                                 \
		})

    /// Fails a test with a given reason.
	#define FAIL_TEST(reason)             \
		({                                \
			__context->status = FAILED;   \
			__context->message = #reason; \
			return;                       \
		})

    /// Register test_suite with a set of test names passed inside curly brackets
	#define TEST_SUITE(test_suite, ...)                                                                        \
		static TasteTest_t* (*TASTE_TEST_SUITE_NAME(test_suite)[])() = __VA_ARGS__;                            \
		void TASTE_TEST_SUITE_REG_NAME(test_suite)() {                                                         \
			const long test_count = sizeof((TasteTest_t * (*[])()) __VA_ARGS__) / sizeof(TasteTest_t * (*)()); \
			register_test_suite(TASTE_TEST_SUITE_NAME(test_suite), test_count, #test_suite);                   \
		}

    /// Include test_suite in testing
	#define INCLUDE_SUITE(test_suite)                 \
		void TASTE_TEST_SUITE_REG_NAME(test_suite)(); \
		TASTE_TEST_SUITE_REG_NAME(test_suite)();

#endif
