#include <mallo_cer.h>

void     ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	        ft_putstr_fd(char *s, int fd)
{
    while (*s)
    {
        ft_putchar_fd(*s++, fd);
    }
}

void	        ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		ft_putstr_fd("-2147483648", fd);
	else if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
	else if (n < 10 && n != -2147483648)
		ft_putchar_fd(n + '0', fd);
}

void	ft_puthex_fd(unsigned long long num, int fd)
{
	if (num >= 16)
	{
		ft_puthex_fd(num / 16, fd);
		ft_puthex_fd(num % 16, fd);
	}
	else
	{
		if (num < 10)
			ft_putnbr_fd(num, fd);
		else
		{
            ft_putchar_fd('a' + num - 10, fd);
		}
	}
}

void	ft_putptr_fd(void *ptr, int fd)
{
	if (!ptr)
		ft_putstr_fd("0x0", fd);
	else
	{
		ft_putstr_fd("0x", fd);
		ft_puthex_fd(((unsigned long long int)ptr), 0);
	}
}
