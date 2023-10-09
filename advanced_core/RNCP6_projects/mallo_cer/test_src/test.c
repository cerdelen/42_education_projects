#include <mallo_cer.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

void        fill_malloc_state(size_t* state)
{
    t_pile*     pile = g_pile_anchor;
    t_chunk*    chunk;
    state[0] = 0;
    state[1] = 0;
    state[2] = 0;
    while (pile != NULL)
    {
        state[0]++;
        chunk = pile->first_chunk;
        while (chunk != NULL)
        {
            if (chunk->free == false)
                state[2]++;
            state[1]++;
            chunk = chunk->next;
        }
        pile = pile->next;
    }
}

void        print_malloc_state()
{
    size_t      state[3];

    fill_malloc_state(state);
    ft_putstr_fd(ON_PURPLE_TERMINAL_COL, 1);
    ft_putstr_fd("Summary: ", 1);
    ft_putstr_fd(DEFAULT_TERMINAL_COL, 1);
    ft_putstr_fd("\n", 1);
    ft_putstr_fd(ON_PURPLE_TERMINAL_COL, 1);
    ft_putstr_fd("Piles: ", 1);
    ft_putnbr_fd(state[0], 1);
    ft_putstr_fd(", Chunks: ", 1);
    ft_putnbr_fd(state[1], 1);
    ft_putstr_fd(", Allocated: ", 1);
    ft_putnbr_fd(state[2], 1);
    ft_putstr_fd(DEFAULT_TERMINAL_COL, 1);
    ft_putstr_fd("\n", 1);
}

void        print_expectations(size_t Piles, size_t Chunks, size_t Alloc, char *col)
{
    ft_putstr_fd(col, 1);
    ft_putstr_fd("Expecting:", 1);
    ft_putstr_fd(DEFAULT_TERMINAL_COL, 1);
    ft_putstr_fd("\n", 1);
    ft_putstr_fd(col, 1);
    ft_putstr_fd("Piles: ", 1);
    ft_putnbr_fd(Piles, 1);
    ft_putstr_fd(", Chunks: ", 1);
    ft_putnbr_fd(Chunks, 1);
    ft_putstr_fd(", Allocated: ", 1);
    ft_putnbr_fd(Alloc, 1);
    ft_putstr_fd(DEFAULT_TERMINAL_COL, 1);
    ft_putstr_fd("\n", 1);
}

bool        compare_ex_to_state(size_t* expected, size_t* state)
{
    if (expected[0] != state[0] || expected[1] != state[1] || expected[2] != state[2])
        return (false);
    return (true);
}

void        check_validity_of_state(size_t Piles, size_t Chunks, size_t Alloc, char *col)
{
    size_t      state[3];
    size_t      expected[3] = {Piles, Chunks, Alloc};

    fill_malloc_state(state);
    if (compare_ex_to_state(expected, state) == false)
    {
        ft_putstr_fd(RED_TERMINAL_COL, 1);
        ft_putstr_fd("TEST FAILED", 1);
        ft_putstr_fd(DEFAULT_TERMINAL_COL, 1);
        ft_putstr_fd("\n", 1);
        print_expectations(Piles, Chunks, Alloc, col);
        print_malloc_state();
        return ;
    }
    ft_putstr_fd(GREEN_TERMINAL_COL, 1);
    ft_putstr_fd("TEST PASSED", 1);
    ft_putstr_fd(DEFAULT_TERMINAL_COL, 1);
    ft_putstr_fd("\n", 1);
}

void        test_1()
{
    void* ptr = malloc(1);
    void* ptr1 = malloc(1);
    void* ptr2 = malloc(17);
    void* ptr3 = malloc(17);

    check_validity_of_state(1, 5, 4, ON_PINK_TERMINAL_COL);

    free(ptr);
    free(ptr1);
    free(ptr2);
    free(ptr3);

    check_validity_of_state(0, 0, 0, ON_PINK_TERMINAL_COL);

}

void    test_2()
{
    void* ptr0 = malloc(1);
    void* ptr1 = malloc(1);
    void* ptr2 = malloc(17);
    void* ptr3 = malloc(17);

    check_validity_of_state(1, 5, 4, ON_PINK_TERMINAL_COL);

    free(ptr2);
    free(ptr0);
    check_validity_of_state(1, 5, 2, ON_PINK_TERMINAL_COL);
    free(ptr3);
    check_validity_of_state(1, 3, 1, ON_PINK_TERMINAL_COL);
    free(ptr1);

    check_validity_of_state(0, 0, 0, ON_PINK_TERMINAL_COL);
}

void    test_3()
{
    void* ptr0 = malloc(1);
    void* ptr1 = malloc(SMALL_CHUNK);
    void* ptr2 = malloc(BIG_CHUNK);

    check_validity_of_state(3, 5, 3, ON_PINK_TERMINAL_COL);

    free(ptr0);
    check_validity_of_state(2, 3, 2, ON_PINK_TERMINAL_COL);
    free(ptr1);
    check_validity_of_state(1, 1, 1, ON_PINK_TERMINAL_COL);
    free(ptr2);
    check_validity_of_state(0, 0, 0, ON_PINK_TERMINAL_COL);
}

void    test_4()
{
    void* ptr0 = malloc(1);

    check_validity_of_state(1, 2, 1, ON_PINK_TERMINAL_COL);

    free(ptr0);

    check_validity_of_state(0, 0, 0, ON_PINK_TERMINAL_COL);

    ptr0 = malloc(1);

    check_validity_of_state(1, 2, 1, ON_PINK_TERMINAL_COL);

    free(ptr0);

    check_validity_of_state(0, 0, 0, ON_PINK_TERMINAL_COL);
}

void    test_5()
{
    void*   ptr0 = malloc(1);

    check_validity_of_state(1, 2, 1, ON_PINK_TERMINAL_COL);

    ptr0 = realloc(ptr0, 17);

    check_validity_of_state(1, 2, 1, ON_PINK_TERMINAL_COL);

    free(ptr0);

    check_validity_of_state(0, 0, 0, ON_PINK_TERMINAL_COL);
}

void    test_6()
{
    void*   ptrs[100];
    void*   ptrb[100];
    void*   ptrh[100];

    for (int i = 0; i < 100; i++)
    {
        ptrs[i] = malloc(1);
        ptrb[i] = malloc(SMALL_CHUNK + 1);
        ptrh[i] = malloc(BIG_CHUNK + 1);
    }
    
    check_validity_of_state(102, 302, 300, ON_PINK_TERMINAL_COL);

    for (int i = 0; i < 100; i++)
    {
        free(ptrs[i]);
        free(ptrb[i]);
        free(ptrh[i]);
    }

    check_validity_of_state(0, 0, 0, ON_PINK_TERMINAL_COL);

}

void    test_7()
{
    void*   ptr0 = malloc(1);
    void*   ptr1 = malloc(1);
    ptr0 = realloc(ptr0, 17);
    (void) ptr1;

    check_validity_of_state(1, 4, 2, ON_PINK_TERMINAL_COL);

    free(ptr0);
    free(ptr1);

    check_validity_of_state(0, 0, 0, ON_PINK_TERMINAL_COL);
}

void    test_8()
{
    void*   ptr0 = malloc(48);
    void*   ptr1 = malloc(1);
    void*   ptr2 = malloc(1);

    free(ptr0);

    check_validity_of_state(1, 4, 2, ON_PINK_TERMINAL_COL);

    ptr1 = realloc(ptr1, 17);

    check_validity_of_state(1, 3, 1, ON_PINK_TERMINAL_COL);

    free(ptr1);
    free(ptr2);

    check_validity_of_state(0, 0, 0, ON_PINK_TERMINAL_COL);
}

int main(int argc, char **argv)
{
    char*   arg = "-a";
    if (argc == 2)
        arg = argv[1];
    if (ft_strncmp(arg, "-a", ft_strlen(arg)) == 0 || ft_strncmp(arg, "1", ft_strlen(arg)) == 0)
        test_1();
    if (ft_strncmp(arg, "-a", ft_strlen(arg)) == 0 || ft_strncmp(arg, "2", ft_strlen(arg)) == 0)
        test_2();
    if (ft_strncmp(arg, "-a", ft_strlen(arg)) == 0 || ft_strncmp(arg, "3", ft_strlen(arg)) == 0)
        test_3();
    if (ft_strncmp(arg, "-a", ft_strlen(arg)) == 0 || ft_strncmp(arg, "4", ft_strlen(arg)) == 0)
        test_4();
    if (ft_strncmp(arg, "-a", ft_strlen(arg)) == 0 || ft_strncmp(arg, "5", ft_strlen(arg)) == 0)
        test_5();
    if (ft_strncmp(arg, "-a", ft_strlen(arg)) == 0 || ft_strncmp(arg, "6", ft_strlen(arg)) == 0)
        test_6();
    if (ft_strncmp(arg, "-a", ft_strlen(arg)) == 0 || ft_strncmp(arg, "7", ft_strlen(arg)) == 0)
        test_7();

    return (0);
}
