#include <mallo_cer.h>
#include <stdio.h>



void*       ptr_pile_data_end(t_pile* pile)
{
    void*       ret = NULL;
    t_chunk*    chunk = NULL;

    if (pile)
    {
        chunk = pile->first_chunk;
        while (chunk->next)
        {
            chunk = chunk->next;
        }
        ret = (void*) chunk + ALIGNED_SIZEOF_CHUNK + chunk->size;
    }
    return (ret);
}

void        print_chunks(t_chunk* chunk, int chunk_idx)
{
    ft_putstr_fd(BLUE_TERMINAL_COL, 1);
    // ft_putstr_fd("|\n", 1);
    // ft_putstr_fd(" ---Chunk idx: ", 1);
    ft_putstr_fd("\tChunk idx: ", 1);
    ft_putnbr_fd(chunk_idx, 1);
    // ft_putstr_fd(", size: ", 1);
    // ft_putnbr_fd(chunk->size, 1);
    ft_putstr_fd(", ", 1);
    if (chunk->free)
    {
        ft_putstr_fd(GREEN_TERMINAL_COL, 1);
        ft_putstr_fd("FREE", 1);
        ft_putstr_fd("\t\t\t\t\t", 1);
    }
    else
    {
        ft_putstr_fd(RED_TERMINAL_COL, 1);
        ft_putstr_fd("ALLOCATED", 1);
        ft_putstr_fd("\t\t\t\t\t", 1);
    }
    ft_putstr_fd(DEFAULT_TERMINAL_COL, 1);
    ft_putptr_fd((void*)chunk, 1);
    ft_putstr_fd("\t", 1);
    ft_putnbr_fd(ALIGNED_SIZEOF_CHUNK, 1);
    ft_putstr_fd("\t\t", 1);
    ft_putptr_fd((void*)chunk + ALIGNED_SIZEOF_CHUNK, 1);
    ft_putstr_fd("\t", 1);
    ft_putptr_fd((void*)chunk + ALIGNED_SIZEOF_CHUNK + chunk->size, 1);
    ft_putstr_fd("\t", 1);
    ft_putnbr_fd(chunk->size, 1);
    ft_putstr_fd("\n", 1);
}

void        pile_announce(t_pile* pile)
{
    static int  pile_nr = 0;

    ft_putstr_fd("\t\t\t\t\t\t\t\tMeta-obj:\tMeta-size:\tData-start:\tData-end:\tData-size:\n", 1);
    ft_putstr_fd(PURPLE_TERMINAL_COL, 1);
    if (pile->chunk_type == SMALL)
    {
        ft_putstr_fd("Small pile, idx: ", 1);
        ft_putnbr_fd(pile_nr, 1);
        ft_putstr_fd(", [ ", 1);
        ft_putnbr_fd(pile->empty_space, 1);
        ft_putstr_fd(" / ", 1);
        ft_putnbr_fd(SMALL_MMAP_SPACE - ALIGNED_SIZEOF_PILE, 1);
        ft_putstr_fd(" ](", 1);
        ft_putnbr_fd((pile->empty_space * 100) / (SMALL_MMAP_SPACE - ALIGNED_SIZEOF_PILE), 1);
        ft_putstr_fd("%)", 1);
    }
    else if (pile->chunk_type == BIG)
    {

        ft_putstr_fd("BIG pile, idx: ", 1);
        ft_putnbr_fd(pile_nr, 1);
        ft_putstr_fd(", [ ", 1);
        ft_putnbr_fd(pile->empty_space, 1);
        ft_putstr_fd(" / ", 1);
        ft_putnbr_fd(BIG_MMAP_SPACE - ALIGNED_SIZEOF_PILE, 1);
        ft_putstr_fd(" ](", 1);
        ft_putnbr_fd((pile->empty_space * 100) / (BIG_MMAP_SPACE - ALIGNED_SIZEOF_PILE), 1);
        ft_putstr_fd("%)", 1);
    }
    else
    {
        ft_putstr_fd("HUMUNGOUS pile, idx: ", 1);
        ft_putnbr_fd(pile_nr, 1);
        ft_putstr_fd(", [ ", 1);
        ft_putnbr_fd(pile->empty_space, 1);
        ft_putstr_fd(" / ", 1);
        ft_putnbr_fd(pile->first_chunk->size + ALIGNED_SIZEOF_CHUNK, 1);
        ft_putstr_fd(" ](0%)", 1);
    }
    ft_putstr_fd(DEFAULT_TERMINAL_COL, 1);
    ft_putstr_fd("\t\t\t\t", 1);
    ft_putptr_fd((void*)pile, 1);
    ft_putstr_fd("\t", 1);
    ft_putnbr_fd(ALIGNED_SIZEOF_PILE, 1);
    ft_putstr_fd("\t\t", 1);
    ft_putptr_fd((void*)pile + ALIGNED_SIZEOF_PILE, 1);
    ft_putstr_fd("\t", 1);
    ft_putptr_fd(ptr_pile_data_end(pile), 1);
    ft_putstr_fd("\t", 1);
    ft_putnbr_fd(ptr_pile_data_end(pile) - ((void*)pile + ALIGNED_SIZEOF_PILE), 1);
    ft_putstr_fd("\n", 1);
    ft_putstr_fd("\n", 1);
}

void        print_pile(t_pile* pile)
{
    t_chunk*    chunk = pile->first_chunk;
    int         chunk_idx = 0;

    pile_announce(pile);
    while(chunk)
    {
        print_chunks(chunk, chunk_idx);
        chunk_idx++;
        chunk = chunk->next;
    }
    ft_putstr_fd("\n", 1);
}

void        show_alloc_mem()
{
    t_pile*     pile = g_pile_anchor;

    // if (pile == NULL)
    // {
    //     ft_putstr_fd(PURPLE_TERMINAL_COL, 1);
    //     ft_putstr_fd("No Piles!\n", 1);
    //     ft_putstr_fd(DEFAULT_TERMINAL_COL, 1);
    //     return ;
    // }
    while(pile != NULL)
    {
        // printf("show loop start  %p\n", pile);
        // printf("show loop start next  %p\n",pile->next);
        print_pile(pile);
        // printf("show loop mid\n");
        pile = pile->next;
        // printf("show loop end\n");
    }
    // printf("show alloc finish\n");
}
