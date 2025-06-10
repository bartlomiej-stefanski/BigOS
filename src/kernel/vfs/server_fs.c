#include "vfs.h"
#include "server_fs.h"
#include "vfs_alloc.h"
#include "file_table.h"
#include <stdbigos/error.h>
#include <stdbigos/pstring.h>


//Currently server files are stored in an array. Finding them by name takes O(MAX_SERVERS) operations.
//Since this number will most likely be small, this should be sufficient, but TODO: perhaps we want something better here anyway
ServerFile_t server_files[MAX_SERVERS];
bool is_server_slot_in_usage[MAX_SERVERS];

void query_queue_init(QueryQueue_t* queue)
{
    queue->head = queue->tail = nullptr;
    queue->size = 0;
}
void query_queue_push(QueryQueue_t* queue, u8 byte)
{
    QueryQueueNode_t* new_node = (QueryQueueNode_t*) vfs_malloc(sizeof(QueryQueueNode_t));
    new_node->key = byte;
    if (queue->size == 0)
    {
        queue->head = queue->tail = new_node;
    }
    else
    {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    queue->size++;
}
error_t query_queue_front(QueryQueue_t* queue, u8* out)
{
    if (!queue->size) return ERR_QUEUE_EMPTY;
    *out = queue->head->key;
    return ERR_NONE;
}

error_t query_queue_pop(QueryQueue_t* queue)
{
    if (!queue->size) return ERR_QUEUE_EMPTY;
    QueryQueueNode_t* old_head = queue->head;
    queue->head = old_head->next;
    vfs_free(old_head);
    queue->size--;
    return ERR_NONE;
}

error_t server_find_by_name(pstring_t *file_name, i64 *out)
{
    i64 idx = -1;
    for (i64 i = 0; i < MAX_SERVERS; i++)
    {
        if ((i64) is_server_slot_in_usage[i] && pstring_strcmp(file_name, &server_files[i].file_name) == 0)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        return ERR_FILE_NOT_FOUND;
    }
    *out = idx;
    return ERR_NONE;
}

error_t server_file_create(pstring_t* file_name, FtEntry_t** out)
{
    i64 free_idx = -1;
    for (i64 i = 0; i < MAX_SERVERS; i++)
    {
        if (!is_server_slot_in_usage[i])
        {
            free_idx = i;
            break;
        }
    }
    if (free_idx == -1)
    {
        return ERR_SERVER_FS_FULL;
    }
    is_server_slot_in_usage[free_idx] = true;
    server_files[free_idx].file_name = *file_name;
    query_queue_init(server_files[free_idx].query_queue);

    *out = ft_add_entry(SERVER_FS_HANDLE, free_idx, 0); //TODO: Attributes
    return ERR_NONE;
}

error_t server_file_open(pstring_t* file_name, FtEntry_t** out)
{
    i64 idx;
    if (server_find_by_name(file_name, &idx) == ERR_FILE_NOT_FOUND)
    {
        return ERR_FILE_NOT_FOUND;
    }
    *out = ft_add_entry(SERVER_FS_HANDLE, idx, 0); //TODO: Attributes
    return ERR_NONE;
}

error_t server_file_read(FtEntry_t* file_entry, u32 bytes, u8* out)
{
    i64 idx = (i64) file_entry->file_id;
    if (!is_server_slot_in_usage[idx]) return ERR_BROKEN_FILE_DESCRIPTOR;
    ServerFile_t* file = &(server_files[idx]);
    for (u32 i = 0; i < bytes; i++)
    {
        u8 byte;
        if (query_queue_front(file->query_queue, &byte) == ERR_QUEUE_EMPTY)
        {
            return ERR_END_OF_FILE; //TODO: Wait for more input? (we need scheduler first)
        }
        query_queue_pop(file->query_queue);
        out[i] = byte;
    }
    return ERR_NONE;
}
error_t server_file_write(FtEntry_t* file_entry, u32 bytes, u8* buff)
{
    i64 idx = (i64) file_entry->file_id;
    if (!is_server_slot_in_usage[idx]) return ERR_BROKEN_FILE_DESCRIPTOR;
    ServerFile_t* file = &(server_files[idx]);
    for (u32 i = 0; i < bytes; i++)
    {
        query_queue_push(file->query_queue, buff[i]);
    }
    return ERR_NONE;
}
error_t server_file_remove(pstring_t* file_name)
{
    i64 idx;
    if (server_find_by_name(file_name, &idx) == ERR_FILE_NOT_FOUND)
    {
        return ERR_FILE_NOT_FOUND;
    }
    is_server_slot_in_usage[idx] = false;
    return ERR_NONE;
}
