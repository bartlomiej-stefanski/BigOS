#ifndef _FILE_TABLE_H_
#define _FILE_TABLE_H_

#include "vfs.h"

FtEntry_t* ft_add_entry(ServiceHandle_t handle, long file_id, int attributes);

#endif
