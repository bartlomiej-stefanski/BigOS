#ifndef _VFS_H_
#define _VFS_H_

#include <stdbigos/types.h>

#include "stdbigos/pstring.h"

// DEBUG for testing CMake configuration
void vfsmain();


#define ServiceHandle_t struct FtEntry_t*

//Usually, service handle is a pointer to a file table entry which corresponds
//to service's communication file. The server-fs driver is the only server
//that doesn't have one (because server-fs driver is the guy who manages
//calls like read or write to communication files, and if it has one too, then
//someone else would have to manage calls to that file). So we place a random number here.

#define SERVER_FS_HANDLE (FtEntry_t*) 39218098419283132

//Table of opened files
typedef struct FtEntry_t
{
    ServiceHandle_t handle;
    u64 file_id;
    u64 attributes;

} FtEntry_t;

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
