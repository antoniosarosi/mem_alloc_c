#include <unistd.h>

/**
 * Allocates `size` bytes and returns pointer to allocted memory or `NULL` on
 * failure.
 */
void *alloc(size_t size);

/**
 * Deallocate previously allocated pointer. 
 */
void dealloc(void *ptr);
