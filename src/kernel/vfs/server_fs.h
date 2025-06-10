#ifndef _SERVER_FS_H_
#define _SERVER_FS_H_

#include <stdbigos/pstring.h>
#include <stdbigos/error.h>
#include "vfs.h"

#define MAX_SERVERS 8

typedef struct QueryQueueNode_t
{
    struct QueryQueueNode_t* next;
    u8 key;
} QueryQueueNode_t;

typedef struct QueryQueue_t
{
    QueryQueueNode_t* head;
    QueryQueueNode_t* tail;
    u64 size;
} QueryQueue_t;

void query_queue_init(QueryQueue_t* queue);
void query_queue_push(QueryQueue_t* queue, u8 c);
error_t query_queue_front(QueryQueue_t* queue, u8* out);
error_t query_queue_pop(QueryQueue_t* queue);

typedef struct ServerFile_t
{
    pstring_t file_name;
    QueryQueue_t* query_queue;
} ServerFile_t;

error_t server_file_create(pstring_t* file_name, FtEntry_t* out);
error_t server_file_open(pstring_t* file_name, FtEntry_t* out);
error_t server_file_read(FtEntry_t* file, u32 bytes, u8* out);
error_t server_file_write(FtEntry_t* file, u32 bytes, u8* buff);

#endif
