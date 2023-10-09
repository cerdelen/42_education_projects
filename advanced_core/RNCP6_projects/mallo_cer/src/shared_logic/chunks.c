#include <mallo_cer.h>
#include <stdio.h>

/*
 *	Takes size and chunk, reserved space in pile and creates next Chunk meta obj
 *	if there is enough space for another chunk after this one
 * */
void			make_reservation(size_t size, t_chunk* chunk, t_pile* parent_pile)
{
	t_chunk*	next = NULL;

	if (chunk->size > size + ALIGNED_SIZEOF_CHUNK)
	{
		next = (void*)chunk + size + ALIGNED_SIZEOF_CHUNK;
		next->prev = chunk;
		next->next = chunk->next;
		next->size = chunk->size - size - ALIGNED_SIZEOF_CHUNK;
		next->free = true;
        parent_pile->empty_space = parent_pile->empty_space - ALIGNED_SIZEOF_CHUNK;
	}
	chunk->size = size;
	chunk->next = next;
	chunk->free = false;

    parent_pile->empty_space = parent_pile->empty_space - size;
    // printf("Reservation made: \nfirst chunk:\t\t%p \ndata:\t\t\t%p \nnext:\t\t\t%p \nnext.data:\t\t%p\n",
    //        chunk, (void*)chunk + ALIGNED_SIZEOF_CHUNK, chunk->next, (void*)chunk->next + ALIGNED_SIZEOF_CHUNK);
    //
    // printf("%x\n", (unsigned int)chunk->size);
    //
    // printf("\n\ndiff of 1\n%p\n%p\n\n", chunk, (void*)chunk + 1);
}

/*
 *	Returns ptr after Chunk Meta Data
 * */
void*			get_chunk_from_specific_pile(size_t size, t_pile *pile)
{
	if (size > pile->empty_space + ALIGNED_SIZEOF_CHUNK)
		return (NULL);
	t_chunk*	chunk = pile->first_chunk;

	while (chunk)
	{
		if (chunk->free == true && chunk->size > size)
		{
			make_reservation(size, chunk, pile);
			return ((void*)chunk + ALIGNED_SIZEOF_CHUNK);
		}
		chunk = chunk->next;
	}
	return (NULL);
}

/*
 *	Merges 2 Chunks (Rh is merged into Lh)
 *	Does not touch the free field
 * */
void			merge_chunks(t_chunk* lh, t_chunk* rh, t_pile* parent_pile)
{
	if (lh == NULL || rh == NULL)
		return ;
	if (lh > rh)
		ptr_swap(lh, rh);
	lh->next = rh->next;
	if (lh->next != NULL)
		lh->next->prev = lh;
	lh->size = lh->size + rh->size + ALIGNED_SIZEOF_CHUNK;
    parent_pile->empty_space += ALIGNED_SIZEOF_CHUNK;
}

/*
 *	Iterates through all existing piles and checks if it can provide 
 *	the space required
 * */
void*			get_chunk_from_piles(size_t size, t_chunk_type chunk_type)
{
	if (chunk_type == HUMUNGOUS || g_pile_anchor == NULL)
		return (NULL);
	void*		ptr = NULL;
	t_pile*		pile_ptr = g_pile_anchor;
	while (pile_ptr && ptr == NULL)
	{
		if (pile_ptr->chunk_type == chunk_type)
			ptr = get_chunk_from_specific_pile(size, pile_ptr);
		pile_ptr = pile_ptr->next;
	}
	return (ptr);
}

t_chunk_type	get_chunk_type(size_t size)
{
	if (size < SMALL_CHUNK)
		return (SMALL);
	if (size < BIG_CHUNK)
		return (BIG);
	return (HUMUNGOUS);
}

t_chunk*       find_chunk(void* ptr)
{
    if (ptr == NULL)
        return NULL;
    t_pile*     pile = g_pile_anchor;
    t_chunk*    chunk = NULL;

    while (pile)
    {
        // printf("pile: %p\n", pile);
        if ((void*)pile < ptr)
        {
            chunk = check_if_pile_contains_chunk(pile, ptr);
            // printf("chunk: %p\n", chunk);
            if (chunk != NULL)
                return (chunk);
        }
        pile = pile->next;
    }
    return (NULL);
}

void        free_chunk(t_chunk* chunk, t_pile* parent_pile)
{
    parent_pile->empty_space += chunk->size;
    if (chunk->next)
    {
        if (chunk->next->free == true)
            merge_chunks(chunk, chunk->next, parent_pile);
    }
    if(chunk->prev)
    {
        if (chunk->prev->free == true)
            merge_chunks(chunk->prev, chunk, parent_pile);
    }
    chunk->free = true;
}

void        shrink_chunk(t_chunk* chunk, size_t size)
{
    if (chunk->size < size + ALIGNED_SIZEOF_CHUNK)
        return ;

    t_chunk*    next = (void*)chunk + size + ALIGNED_SIZEOF_CHUNK;
    next->size = chunk->size - (ALIGNED_SIZEOF_CHUNK + size);
    next->free = true;
    next->next = chunk->next;
    next->prev = chunk;

    if (next->next != NULL)
        next->next->prev = next;

    chunk->next = next;
    chunk->size = size;
}
