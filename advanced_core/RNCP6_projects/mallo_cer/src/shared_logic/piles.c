#include "mallo_cer.h"
#include <stdio.h>

void*		create_new_pile(size_t size, t_chunk_type chunk_type)
{
	if (chunk_type == HUMUNGOUS)
		return (create_humungous_new_pile(size));
	return (create_new_pile_s_or_b(chunk_type));
}

void*		create_new_pile_s_or_b(t_chunk_type chunk_type)
{
	t_pile*		new_pile;
	size_t		space;
	if (chunk_type == SMALL)
		space = SMALL_MMAP_SPACE;
	else
		space = BIG_MMAP_SPACE;

	new_pile = (t_pile*)mmap(NULL,
						space,
						PROT_READ | PROT_WRITE,
						MAP_PRIVATE | MAP_ANON,
						-1, 0);

	new_pile->chunk_type = chunk_type;
	new_pile->first_chunk = (void*)new_pile + ALIGNED_SIZEOF_PILE;
	new_pile->next = g_pile_anchor;
	new_pile->empty_space = space - (ALIGNED_SIZEOF_PILE + ALIGNED_SIZEOF_CHUNK);
	new_pile->prev = NULL;

	new_pile->first_chunk->prev = NULL;
	new_pile->first_chunk->next = NULL;
	new_pile->first_chunk->size = space - (ALIGNED_SIZEOF_PILE + ALIGNED_SIZEOF_CHUNK);
	new_pile->first_chunk->free = true;

    if (g_pile_anchor != NULL)
        g_pile_anchor->prev = new_pile;
	g_pile_anchor = new_pile;

	return (new_pile);
}

void*		create_humungous_new_pile(size_t size)
{
	t_pile*		new_pile = (t_pile*)mmap(NULL,
								size + ALIGNED_SIZEOF_PILE + ALIGNED_SIZEOF_CHUNK,
								PROT_READ | PROT_WRITE,
								MAP_PRIVATE | MAP_ANON,
								-1, 0);

	new_pile->chunk_type = HUMUNGOUS;
	new_pile->first_chunk = (void*)new_pile + ALIGNED_SIZEOF_PILE;
	new_pile->next = g_pile_anchor;
	new_pile->empty_space = 0;
	new_pile->prev = NULL;

	new_pile->first_chunk->prev = NULL;
	new_pile->first_chunk->next = NULL;
	new_pile->first_chunk->size = size;

    if (g_pile_anchor != NULL)
        g_pile_anchor->prev = new_pile;
    g_pile_anchor = new_pile;

	return (new_pile);
}

t_pile*     find_parent_pile(t_chunk* chunk)
{
    t_chunk*    tmp = chunk;

    while(tmp->prev != NULL)
        tmp = tmp->prev;
    return ((void*) tmp - ALIGNED_SIZEOF_PILE);
}

t_chunk*    check_if_pile_contains_chunk(t_pile* pile, void* ptr)
{
    t_chunk*    chunk = pile->first_chunk;

    while (chunk)
    {
        if (((void*)chunk + ALIGNED_SIZEOF_CHUNK) == ptr)
            return (chunk);
        chunk = chunk->next;
    }
    return (NULL);
}

void        free_pile(t_pile* pile)
{
    if (pile == g_pile_anchor)
        g_pile_anchor = pile->next;
    if (pile->next != NULL)
        pile->next->prev = pile->prev;
    if (pile->prev != NULL)
        pile->prev->next = pile->next;
    munmap(pile, pile->first_chunk->size + ALIGNED_SIZEOF_PILE);
}
