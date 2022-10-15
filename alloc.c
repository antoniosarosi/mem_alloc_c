#include "alloc.h"

// Block header. Contains metadata about the allocated block.
typedef struct Header {
    // Allocated size, excluding header size.
    size_t size;
    // Whether the block is currently free or not.
    unsigned is_free;
    // Next block.
    struct Header *next;
} Header;

// First allocated block.
Header *first = NULL;

// Last allocated block.
Header *last = NULL;

// Returns the machine word size alignment for the given value. 
static inline size_t align(size_t to_be_aligned) {
    return (to_be_aligned + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);
}

// Returns the first block that can hold `size` amount of bytes and is free.
Header *find_free_block(size_t size) {
    Header *current = first;
    
    while (current != NULL) {
        if (current->size <= size && current->is_free) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

void *alloc(size_t size) {
    // First try to find a free block that has been deallocated and has enough
    // capacity to store `size` bytes.
    Header *free_block = find_free_block(size);
    if (free_block != NULL) {
        // We are lucky and we don't have to do much work.
        free_block->is_free = 0;
        // The pointer where data can be written is located _after_ the header
        return ((void *) free_block) + sizeof(Header);
    }

    // If we didn't find any free blocks, we have to increment the program
    // break and allocate a new block.

    // The actual size that we need is the requested size + header size.
    // It should also be aligned for faster memory access.
    size_t total_size = align(sizeof(Header) + size);

    // sbrk is a syscall that can increment or decrement the _program break_.
    // It is obsolete and mmap is recommended instead, but sbrk is simpler.
    void *addr = sbrk(total_size);

    // Return NULL if sbrk fails
    if (addr == (void*) -1) {
        return NULL;
    }

    // Write the header contents at the beginning of the heap address
    Header *header = addr;
    header->size = size;
    header->is_free = 0;
    header->next = NULL;

    // Update linked list
    if (first == NULL) {
        first = header;
        last = header;
    } else {
        last->next = header;
        last = last->next;
    }

    // The pointer where data can be written is located _after_ the header
    return addr + sizeof(Header);
}

void dealloc(void *ptr) {
    // The actual address of the block is the given address - size of header
    // because when we allocate memory we return a pointer that points _after_
    // the header.
    Header *header = ptr - sizeof(Header);

    // Mark the block as free to use.
    header->is_free = 1;

    // If the block is not the last block allocated, we can't really do much
    // because we cannot remove intermediate blocks, we can only decrement
    // the program break, so we can only release blocks in reverse order.
    if (header != last) {
        return;
    }

    // On the other hand, if we are so lucky that the block to be deallocated
    // is the last one, then we can release that memory back to the OS. So,
    // first search the previous block to update the tail in the linked list.
    if (first == last) {
        first = NULL;
        last = NULL;
    } else {
        Header *current = first;
        while (current->next != NULL && current->next != last) {
            current = current->next;
        }
        last = current;
    }

    // Now decrement program counter.
    sbrk(0 - header->size - sizeof(Header));
}
