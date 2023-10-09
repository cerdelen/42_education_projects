#include <mallo_cer.h>

t_chunk*        realloc_merge_current_with_next(t_chunk* chunk, size_t size)
{
    t_chunk*    new_next = (void*)chunk + size + ALIGNED_SIZEOF_CHUNK;
    t_chunk*    next = chunk->next;
    size_t      av_size = chunk->size + next->size + ALIGNED_SIZEOF_CHUNK;

    new_next->next = next->next;
    new_next->prev = chunk;
    new_next->size = av_size - ALIGNED_SIZEOF_CHUNK - size;
    new_next->free = true;

    chunk->next = new_next;
    chunk->size = size;

    return (chunk);
}

t_chunk*        realloc_merge_current_with_prev(t_chunk* chunk, size_t size)
{
    t_chunk*    dst = chunk->prev;

    dst->size = size;
    dst->next = chunk->next;
    dst->free = false;
    ft_memmove((void*)dst + ALIGNED_SIZEOF_CHUNK, (void*)chunk + ALIGNED_SIZEOF_CHUNK, chunk->size);

    t_chunk*    new_next = (void*) dst + ALIGNED_SIZEOF_CHUNK + size;
    new_next->prev = dst;
    new_next->next = dst->next;
    new_next->size = (void*)new_next->next - ((void*)new_next + ALIGNED_SIZEOF_CHUNK);
    new_next->free = true;

    if (new_next->next != NULL)
        new_next->next->prev = new_next;

    dst->next = new_next;

    return (dst);
}

t_chunk*        realloc_merge_current_with_next_and_prev(t_chunk* chunk, size_t size) // guaranteed prev and next
{
    t_chunk*    dst = chunk->prev;

    dst->size = size;
    dst->free = false;
    dst->next = chunk->next->next;  // tmp stored so i dont loose it

    ft_memmove((void*)dst + ALIGNED_SIZEOF_CHUNK, (void*)chunk + ALIGNED_SIZEOF_CHUNK, chunk->size);

    t_chunk*    new_next = (void*) dst + ALIGNED_SIZEOF_CHUNK + size;
    new_next->prev = dst;
    new_next->next = dst->next;
    new_next->size = (void*)new_next->next - ((void*)new_next + ALIGNED_SIZEOF_CHUNK);
    new_next->free = true;

    if (new_next->next != NULL)
        new_next->next->prev = new_next;

    dst->next = new_next;

    return (dst);
}

void*           relocate_chunk(t_chunk* chunk, size_t size)
{
    if (chunk->next != NULL)
    {
        if (chunk->next->free == true)
        {
            if ((chunk->size + chunk->next->size + ALIGNED_SIZEOF_CHUNK) >= size)
                return (realloc_merge_current_with_next(chunk, size));
        }
        if (chunk->prev != NULL)
        {
            if ((chunk->size + chunk->prev->size + ALIGNED_SIZEOF_CHUNK) >= size)
                return (realloc_merge_current_with_prev(chunk, size));
        }
        if (chunk->prev != NULL && chunk->next != NULL)
        {
            if (chunk->prev->free == true && chunk->next->free)
            {
                if ((chunk->size +
                    chunk->next->size + ALIGNED_SIZEOF_CHUNK +
                    chunk->prev->size + ALIGNED_SIZEOF_CHUNK) >= size)
                    realloc_merge_current_with_next_and_prev(chunk, size);
            }
        }
        void*   ptr = malloc(size);
        if (ptr == NULL)
            return (NULL);

        ft_memmove((void*)chunk + ALIGNED_SIZEOF_CHUNK, ptr, chunk->size);
        free_chunk(chunk, find_parent_pile(chunk));
        return ((void*)ptr - ALIGNED_SIZEOF_CHUNK);
    }
    return (NULL);
}

/*
 *  
     The realloc() function tries to change the size of the allocation pointed to by ptr to size, and returns ptr.  If there is not enough room
     to enlarge the memory allocation pointed to by ptr, realloc() creates a new allocation, copies as much of the old data pointed to by ptr as
     will fit to the new allocation, frees the old allocation, and returns a pointer to the allocated memory.  If ptr is NULL, realloc() is
     identical to a call to malloc() for size bytes.  If size is zero and ptr is not NULL, a new, minimum sized object is allocated and the
     original object is freed.  When extending a region allocated with calloc(3), realloc(3) does not guarantee that the additional memory is
     also zero-filled.
 * */
void*           realloc(void *ptr, size_t size)
{
    if (ptr == NULL)    
        return (malloc(size));
    t_chunk*    chunk = find_chunk(ptr);

    if (chunk == NULL)
        return (malloc(size));

    size = (size + 15) &~15;

    if (size < chunk->size)
    {
        // make ptr smalles size possible (just size 0, so only meta data? or 16 because of alignment)
        shrink_chunk(chunk, size);
        return ((void*)chunk + ALIGNED_SIZEOF_CHUNK);
    }
    chunk = relocate_chunk(chunk, size);
    if (chunk != NULL)
        return ((void*)chunk + ALIGNED_SIZEOF_CHUNK);
    return (NULL);
}
