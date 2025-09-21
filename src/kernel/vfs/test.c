#ifdef TASTE_TESTS

	#include <stdbigos/error.h>
	#include <stdbigos/pstring.h>
	#include <stdbigos/types.h>
	#include <taste/test.h>

	#include "file_table.h"
	#include "pipes.h"
	#include "vfs.h"
	#include "vfs_alloc.h"

TEST(test_path_next) {
	pstring_t path1 = ERRX_UNWRAP(pstring_l2w("/foo/bar/file.c"));
	VfsPath_t path = vfs_path_new(&path1);
	pstring_t expected;
	int_or_err_t cmp_result;
	pstring_t edge = {.len = 0, .data = nullptr};

	expected = ERRX_UNWRAP(pstring_l2w("/"));
	ASSERT_TRUE(vfs_path_next(&path, &edge), "Should pick up the root directory '/'");
	cmp_result = pstring_strcmp(&edge, &expected);
	ASSERT_EQ(cmp_result.err, ERR_NONE, "Compare with '/' should not fail");
	ASSERT_EQ(cmp_result.val, 0, "Compare with '/' should be equal");

	expected = ERRX_UNWRAP(pstring_l2w("foo"));
	ASSERT_TRUE(vfs_path_next(&path, &edge), "Should pick up 'foo' directory");
	cmp_result = pstring_strcmp(&edge, &expected);
	ASSERT_EQ(cmp_result.err, ERR_NONE, "Compare with 'foo' should not fail");
	ASSERT_EQ(cmp_result.val, 0, "Compare with 'bar' should be equal");

	expected = ERRX_UNWRAP(pstring_l2w("bar"));
	ASSERT_TRUE(vfs_path_next(&path, &edge), "Should pick up 'bar' directory");
	cmp_result = pstring_strcmp(&edge, &expected);
	ASSERT_EQ(cmp_result.err, ERR_NONE, "Compare with 'bar' should not fail");
	ASSERT_EQ(cmp_result.val, 0, "Compare with 'bar' should be equal");

	expected = ERRX_UNWRAP(pstring_l2w("file.c"));
	ASSERT_TRUE(vfs_path_next(&path, &edge), "Should pick up 'file.c' file");
	cmp_result = pstring_strcmp(&edge, &expected);
	ASSERT_EQ(cmp_result.err, ERR_NONE, "Compare with 'file.c' should not fail");
	ASSERT_EQ(cmp_result.val, 0, "Compare with 'file.c' should be equal");

	ASSERT_TRUE(!vfs_path_next(&path, &edge), "Return false after path end");
}

TEST(test_pipes) {
	FtEntry_t* example_file_entry1;
	FtEntry_t* example_file_entry2;
	size_or_err_t result;

	// Find a space to place these structs in user-process-memory
	example_file_entry1 = ft_add_entry();
	example_file_entry2 = ft_add_entry();

	// Create a pipe that connects these file entries
	ASSERT_EQ(pipe_create(&example_file_entry1->kernel_read_pipe, &example_file_entry2->kernel_write_pipe), ERR_NONE,
	          "Pipe creation should not fail");

	const pstring_t message1 =
	    ERRX_UNWRAP(pstring_l2w("Hello from vfs! This message is brought to you via a Kernel Pipe.\n"));
	const pstring_t message2 =
	    ERRX_UNWRAP(pstring_l2w("Hello again, it seems that these pipes are not just for one time use!\n"));
	const pstring_t message3 = ERRX_UNWRAP(pstring_l2w("How convenient!\n"));

	// TODO: Figure out better way of doing 'this' with pstring
	pstring_t buff = (pstring_t){
	    .len = 100,
	    .data = vfs_alloca(100),
	};

	// Write and read a mesage
	result = pipe_write(&example_file_entry2->kernel_write_pipe, &message1);
	ASSERT_EQ(result.err, ERR_NONE, "First write to pipe should not fail");
	ASSERT_EQ(result.val, message1.len, "First write to pipe shuold write all");
	result = pipe_read(&example_file_entry1->kernel_read_pipe, &buff);
	ASSERT_EQ(result.err, ERR_NONE, "First read from pipe should not fail");
	ASSERT_EQ(result.val, message1.len, "First read from pipe should read all");

	// Create a buffer too small to read whole next message
	buff.len = 20;

	// Write two messages at once and then read them with different sizes
	result = pipe_write(&example_file_entry2->kernel_write_pipe, &message2);
	ASSERT_EQ(result.err, ERR_NONE, "Second write to pipe should not fail");
	ASSERT_EQ(result.val, message2.len, "Second write to pipe shuold write all");
	result = pipe_read(&example_file_entry1->kernel_read_pipe, &buff);
	ASSERT_EQ(result.err, ERR_NONE, "Second read from pipe should not fail");
	ASSERT_EQ(result.val, 20, "Second read from pipe should fill buffer");

	// This buffer will fit the rest of the message
	buff.len = 100;

	result = pipe_write(&example_file_entry2->kernel_write_pipe, &message3);
	ASSERT_EQ(result.err, ERR_NONE, "Third write to pipe should not fail");
	ASSERT_EQ(result.val, message3.len, "Third write to pipe shuold write all");
	result = pipe_read(&example_file_entry1->kernel_read_pipe, &buff);
	ASSERT_EQ(result.err, ERR_NONE, "Third read from pipe should not fail");
	ASSERT_EQ(result.val, message2.len + message3.len - 20,
	          "Third read from pipe should read remaining second message and all of third message");

	// Reading on an empty buffer should not read characters
	result = pipe_read(&example_file_entry1->kernel_read_pipe, &buff);
	ASSERT_EQ(result.err, ERR_NONE, "Read with nothing in buffer shouold not fail");
	ASSERT_EQ(result.val, 0, "Read with nothing in buffer shouold not read anything");

	ft_free_entry(example_file_entry1);
	ft_free_entry(example_file_entry2);
}

TEST_SUITE(vfs, {ALLOWED_TO_FAIL(test_path_next), test_pipes})
#endif
