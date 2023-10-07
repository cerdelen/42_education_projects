#include <mallo_cer.h>

/*
 *	Takes size and chunk, reserved space in pile and creates next Chunk meta obj
 *	if there is enough space for another chunk after this one
 * */
void			make_reservation(size_t size, t_chunk* chunk)
{
	t_chunk*	next = NULL;
	if (chunk->size > size + ALIGNED_SIZEOF_CHUNK)
	{
		next = chunk + size + ALIGNED_SIZEOF_CHUNK;
		next->prev = chunk;
		next->next = chunk->next;
		next->size = chunk->size - size - ALIGNED_SIZEOF_CHUNK;
		next->free = true;
	}
	chunk->size = size;
	chunk->next = next;
	chunk->free = false;
}

/*
 *	Returns ptr after Chunk Meta Data
 * */
void*			get_chunk_from_specific_pile(size_t size, t_pile *pile)
{
	if (size > pile->empty_space + ALIGNED_SIZEOF_CHUNK)
		return (NULL);
	t_chunk*	chunk = pile->first_empty;

	while (chunk)
	{
		if (chunk->free == true && chunk->size > size)
		{
			make_reservation(size, chunk);
			return (chunk + ALIGNED_SIZEOF_CHUNK);
		}
		chunk = chunk->next;
	}
	return (NULL);
}

/*
 *	Merges 2 Chunks (Rh is merged into Lh)
 *	Does not touch the free field
 * */
void			merge_chunks(t_chunk* lh, t_chunk* rh)
{
	if (lh == NULL || rh == NULL)
		return ;
	if (lh > rh)
		ptr_swap(lh, rh);
	lh->next = rh->next;
	if (lh->next != NULL)
		lh->next->prev = lh;
	lh->size = lh->size + rh->size + ALIGNED_SIZEOF_CHUNK;
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
	while (pile_ptr)
	{
		if (g_pile_anchor->chunk_type == chunk_type)
		{
			ptr = get_chunk_from_specific_pile(size, pile_ptr);
		}
		pile_ptr = pile_ptr->next;
	}
	return (ptr);
}



t_chunk_type	get_chunk_type(size_t size)
{
	if (size < SMALL)
		return (SMALL);
	if (size < BIG)
		return (BIG);
	return (HUMUNGOUS);
}
