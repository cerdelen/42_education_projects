#include <mallo_cer.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;
	int				r;
	unsigned char	*a1;
	unsigned char	*a2;

	a1 = (unsigned char *) s1;
	a2 = (unsigned char *) s2;
	i = 0;
	while (i < n)
	{
		if (a1[i] != a2[i])
		{
			r = a1[i] - a2[i];
			return (r);
		}
		if (a1[i] == '\0' && a2[i] == '\0')
		{
			r = 0;
			return (r);
		}
		i++;
	}
	r = 0;
	return (r);
}

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
