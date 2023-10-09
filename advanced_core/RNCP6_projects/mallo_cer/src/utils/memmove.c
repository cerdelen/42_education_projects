#include <mallo_cer.h>

static void    copy_forward(void *dst, const void* src, size_t n)
{
    while (n > 0)
    {
        *(char*)dst++ = *(char*)src++;
        n--;
    }
}

static void    copy_backwards(void *dst, const void* src, size_t n)
{
    dst = dst + n;
    src = src + n;
    while (n > 0)
    {
        *(char*)dst-- = *(char*)src--;
        n--;
    }
}

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	if (!dst && !src)
		return (NULL);
	if (dst > src)
        copy_backwards(dst, src, n);
	else
        copy_forward(dst, src, n);
	return (dst);
}
