#include <mallo_cer.h>

void        print_digit(unsigned char c)
{
    if (c> 9)
        ft_putchar_fd('a' + (c - 10), 1);
    else
        ft_putnbr_fd(c, 1);
}
void        dump_hex(unsigned char val)
{
    ft_putstr_fd("0x", 1);
    print_digit(val % 16);
    print_digit(val / 16);
}

void        hex_dump_chunk(t_chunk* chunk)
{
    if (chunk->free == true)
        return ;

    char*   val = (void*)chunk + ALIGNED_SIZEOF_CHUNK;
    int     i = 0;
    ft_putstr_fd("Address: ", 1);
    ft_putptr_fd((void*)chunk + ALIGNED_SIZEOF_CHUNK, 1);
    ft_putstr_fd(", Size: ", 1);
    ft_putnbr_fd(chunk->size, 1);
    while((void*)val != (void*)chunk + ALIGNED_SIZEOF_CHUNK + chunk->size)
    {
        if (i % 8 == 0)
            ft_putstr_fd("\n\t", 1);
        dump_hex(*val);
        ft_putstr_fd(" ", 1);
        i++;
        val++;
    }
    //here hex dump
    ft_putstr_fd("\n", 1);
}

void        show_alloc_mem_ex()
{
    t_pile*     pile = g_pile_anchor;
    t_chunk*    chunk;

    while(pile)
    {
        chunk = pile->first_chunk;
        while(chunk)
        {
            hex_dump_chunk(chunk);
            chunk = chunk->next;
        }
        pile = pile->next;
    }
}
