#ifndef _VFS_H_
#define _VFS_H_

#include <stdbigos/types.h>

#include "stdbigos/pstring.h"

// DEBUG for testing CMake configuration
void vfsmain();
void vfs_init();

// Maybe some metadata here?
typedef struct Service_t {
	pstring_t service_name;
	u64 pipe_id;
} Service_t;

#define ServiceHandle_t Service_t*

// Table of opened files
typedef struct FtEntry_t {
	ServiceHandle_t handle;
	u64 file_id;
	u64 attributes;

} FtEntry_t;

// Pipe description
typedef struct InnerPipeFd_t {
	u64 pipe_id;
	u64 attributes;

} InnerPipeFd_t;

/// Memory-owning representation of path
typedef pstring_t VfsPathBuff_t;

/// View of the VfsPathBuff
typedef struct {
	pstring_t path;
} VfsPath_t;

// TODO: implement
/// Normalizes the path and returns 'owning' reference
// VfsPathBuff_t vfs_path_buff(const pstring_t* path);

VfsPath_t vfs_path_new(const VfsPathBuff_t* path);

/// Iterates over subsequent path elements
bool vfs_path_next(VfsPath_t* path, pstring_t* edge);

#endif
