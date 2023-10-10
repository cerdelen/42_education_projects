#include <mallo_cer.h>
#include <stdio.h>

void        free(void *ptr)
{
    // printf("Called free on %p\n", ptr);
	if (ptr == NULL)
	return ;
	if (lock_mutex_wrapper())
		exit(1);
    t_chunk*    chunk = find_chunk(ptr);

    if (chunk != NULL)
    {
        // printf("found chunk\n");
        t_pile* pile = find_parent_pile(chunk);
        free_chunk(chunk, pile);
        // if (pile->chunk_type == HUMUNGOUS)
        //     printf("ALLOOOOO\n");
        if (pile->first_chunk->next == NULL)
            free_pile(pile);
    }
	if (unlock_mutex_wrapper())
		exit(1);
    // printf("Free failed %p\n", ptr);
}
