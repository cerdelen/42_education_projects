#include <mallo_cer.h>

int main()
{
    size_t*   ptr0 = malloc(sizeof(size_t));
    size_t*   ptr1 = malloc(sizeof(size_t));
    size_t*   ptr2 = malloc(1000);

    *ptr0 = -2;
    *ptr1 = 0;
    show_alloc_mem_ex();
    free(ptr0);
    free(ptr1);
    free(ptr2);
}
