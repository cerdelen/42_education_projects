#include "mallo_cer.h"


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
	new_pile->first_empty = (void*)new_pile + ALIGNED_SIZEOF_PILE;
	new_pile->next = g_pile_anchor;
	new_pile->empty_space = space - ALIGNED_SIZEOF_PILE + ALIGNED_SIZEOF_CHUNK;
	new_pile->prev = NULL;

	new_pile->first_empty->prev = NULL;
	new_pile->first_empty->next = NULL;
	new_pile->first_empty->size = space - (ALIGNED_SIZEOF_PILE + ALIGNED_SIZEOF_CHUNK);

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
	new_pile->first_empty = (void*)new_pile + ALIGNED_SIZEOF_PILE;
	new_pile->next = g_pile_anchor;
	new_pile->empty_space = 0;
	new_pile->prev = NULL;

	new_pile->first_empty->prev = NULL;
	new_pile->first_empty->next = NULL;
	new_pile->first_empty->size = size;

	g_pile_anchor = new_pile;

	return (new_pile);
}
