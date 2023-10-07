#include <mallo_cer.h>
#include <stdio.h>

int main()
{
	printf("T_PILE:\t\t\t\t%lu\n", ALIGNED_SIZEOF_PILE);
	printf("T_CHUNK:\t\t\t%lu\n", ALIGNED_SIZEOF_CHUNK);
	printf("ALIGNMENT:\t\t\t%lu\n", ALIGNMENT);
	printf("SMALL_CHUNK:\t\t\t%lu\n", SMALL_CHUNK);
	printf("BIG_CHUNK:\t\t\t%lu\n", BIG_CHUNK);
	printf("PAGESIZE:\t\t\t%lu\n", PAGESIZE);
	printf("SMALL_MMAP_SPACE:\t\t%lu\n", SMALL_MMAP_SPACE);
	printf("BIG_MMAP_SPACE:\t\t\t%lu\n", BIG_MMAP_SPACE);
}
