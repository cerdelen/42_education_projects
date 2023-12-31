#include <mallo_cer.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void*		get_memory(size_t size)
{
	void*			ptr = NULL;
	void*			new_pile = NULL;
	t_chunk_type	chunk_type = get_chunk_type(size);

    if (size == 0)
		return (NULL);
	size = (size + 15) &~15;
	ptr = get_chunk_from_piles(size, chunk_type);
	if (!ptr)
	{
		new_pile = create_new_pile(size, chunk_type);
        if (chunk_type == HUMUNGOUS)
            return ((void*) new_pile + ALIGNED_SIZEOF_PILE + ALIGNED_SIZEOF_CHUNK);
		ptr = get_chunk_from_specific_pile(size, new_pile);
	}
    // make it so that everything knows what is still free and what is not
    // logging
    // return chunk
    return (ptr);
}

void*		malloc(size_t size)
{
	void*	ret;
	if (lock_mutex_wrapper())
		exit(1);

    // write logging probably
    ret = get_memory(size);
    // set envs and init
	if (unlock_mutex_wrapper())
		exit(1);
    // printf("anchor:\t%p\n", g_pile_anchor);
    // printf("chunk:\t%p\n", g_pile_anchor->first_chunk);
    // printf("data:\t%p\n", (void*)g_pile_anchor->first_chunk + ALIGNED_SIZEOF_CHUNK);
	return (ret);
}
