#include "file_table.h"
#include "vfs.h"
#include "vfs_alloc.h"

FtEntry_t* ft_add_entry(ServiceHandle_t handle, long file_id, int attributes)
{
    FtEntry_t* ret = (FtEntry_t*) vfs_malloc(sizeof(FtEntry_t));
    ret->handle = handle;
    ret->file_id = file_id;
    ret->attributes = attributes;
    return ret;
}
