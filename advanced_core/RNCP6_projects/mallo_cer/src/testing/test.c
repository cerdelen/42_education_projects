#include <mallo_cer.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
    printf("malloc = %p\n", malloc(1));
    return (0);
}
