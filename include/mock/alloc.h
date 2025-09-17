#ifndef _MOCK_ALLOC_H_
#define _MOCK_ALLOC_H_

#include <stddef.h>

/* Simplistic arena allocator.
 * To be used untill a 'true' allocator is available.
 */

#define ALLOCATOR_SIZE 1024 * 1024

void* malloc(size_t size);
void* realloc(void* ptr, size_t new_size);

void free(void* ptr);

#define alloca __builtin_alloca

#endif
