#include "vfs.h"
#include "mount_tree.h"
#include "file_table.h"
#include "server_fs.h"
#include "vfs_alloc.h"

#include <debug/debug_stdio.h>
#include <debug/panic.h>
#include <stdbigos/assert.h>
#include <stdbigos/pstring.h>
#include <stdbigos/string.h>
#include <stddef.h>

// Here just for debugging
void vfsmain() {
	DEBUG_PUTS("Hello VFS\n");
	pstring_t path1 = ERRX_UNWRAP(pstring_l2w("/foo/bar/baz/file.c"));
	VfsPath_t path = vfs_path_new(&path1);

	pstring_t edge;
	edge.data = vfs_malloc(1024);
	if (edge.data == nullptr) {
		PANIC("vfs_malloc failed");
	}
	edge.len = 1024;
	size_t i = 0;
	while (vfs_path_next(&path, &edge)) {
		if (edge.data == nullptr) {
			PANIC("vfs_path_next returned null edge data");
		}
		DEBUG_PRINTF("Path part %zu: ", i++);
		for (size_t i = 0; i < edge.len; i++) {
			DEBUG_PUTC(edge.data[i]);
		}
		DEBUG_PUTC('\n');
	}
	kassert("foo" == nullptr); // Just for testing
	PANIC("This should not be reached");

    FtEntry_t* example_driver;
    FtEntry_t* example_driver2;
    char example_driver_name[] = "example driver";
    pstring_t example_driver_name_pstring = pstring_l2w(example_driver_name).val;
    char buff[100];

    char example_message[] = "Hello server-fs! I'm example driver.\n";
    server_file_create(&example_driver_name_pstring, &example_driver);
    server_file_write(example_driver, strlen(example_message) + 1, (u8*) example_message);
    server_file_read(example_driver, strlen(example_message) + 1, (u8*) buff);
    DEBUG_PUTS(buff);

    char example_message2[] = "Hi there! I'm example driver too!\n";
    server_file_open(&example_driver_name_pstring, &example_driver2);
    server_file_write(example_driver, strlen(example_message2) + 1, (u8*) example_message2);
    server_file_read(example_driver, strlen(example_message2) + 1, (u8*) buff);
    DEBUG_PUTS(buff);
}

VfsPath_t vfs_path_new(const VfsPathBuff_t* path_buff) {
	return (VfsPath_t){.path = *path_buff};
}

bool vfs_path_next(VfsPath_t* path, pstring_t* edge) {
	if (!path || pstring_len_silenterr(&path->path) == 0) {
		return false;
	}

	const pstring_t vpath = path->path;
	pstring_t split = ERRX_UNWRAP(pstring_strchr(&vpath, '/'));
	if (split.len == 0) {
		path->path.data = nullptr;
		path->path.len = 0;
		*edge = vpath;
	}

	size_t split_pos = vpath.len - split.len;
	*edge = ERRX_UNWRAP(pstring_slice_view(&vpath, 0, split_pos));
	path->path = ERRX_UNWRAP(pstring_slice_view(&vpath, split_pos + 1, vpath.len));
	return true;
}
