#include <taste/test.h>

TEST(sanity_check) {
	// Empty test should not fail
}

TEST(assert_eq_works) {
	ASSERT_EQ(2 * 2, 4, "One would expect for 2 * 2 to be 4");
}

TEST(assert_neq_works) {
	ASSERT_NEQ(2 + 2, 5, "One would expect for 2 + 2 to NOT be 5");
}

TEST(assert) {
	ASSERT_TRUE(true, "What is more true than the truth itself?");
}

TEST(assert_eq_can_fail) {
	ASSERT_EQ(2 * 2, 5, "Well, that is not how math works...");
}

TEST(assert_neq_can_fail) {
	ASSERT_NEQ(2 + 2, 4, "Oh but this is just basics...");
}

TEST(assert_can_fail) {
	ASSERT_TRUE(!true, "What is more different to the truth than !true?");
}

TEST_SUITE(sanity_check, {sanity_check, assert_eq_works, assert_neq_works, assert, ALLOWED_TO_FAIL(assert_eq_can_fail),
                          ALLOWED_TO_FAIL(assert_neq_can_fail), ALLOWED_TO_FAIL(assert_can_fail)})
