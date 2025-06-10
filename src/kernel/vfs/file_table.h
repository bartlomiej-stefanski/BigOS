#ifndef _FILE_TABLE_H_
#define _FILE_TABLE_H_

#include "vfs.h"

typedef struct FtEntry_t
{
    ServiceHandle_t handle;
    long file_id;
    int attributes;

} FtEntry_t;

#endif
