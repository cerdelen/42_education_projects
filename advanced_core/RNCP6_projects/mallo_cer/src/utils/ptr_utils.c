#include <mallo_cer.h>

void		ptr_swap(void* ptr1, void* ptr2)
{
	void*	temp = ptr1;
	ptr1 = ptr2;
	ptr2 = temp;
}
