#include "vfs.h"
#include "server_fs.h"
#include "vfs_alloc.h"
#include <stdbigos/error.h>


//Currently server files are stored in an array. Finding them by name takes O(MAX_SERVERS) operations.
//Since this number will most likely be small, this should be sufficient, but TODO: perhaps we want something better here anyway
ServerFile_t server_files[MAX_SERVERS];
bool is_server_slot_in_usage[MAX_SERVERS];

void query_queue_init(QueryQueue_t* queue)
{
    queue->head = queue->tail = nullptr;
    queue->size = 0;
}
void query_queue_push(QueryQueue_t* queue, u8 c)
{
    QueryQueueNode_t* new_node = (QueryQueueNode_t*) vfs_malloc(sizeof(QueryQueueNode_t));
    new_node->key = c;
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
