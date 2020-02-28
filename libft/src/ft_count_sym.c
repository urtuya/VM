#include "libft.h"

size_t	ft_count_sym(int n)
{
	size_t	count;
	int		flag;

	if (n == -2147483648)
		return (11);
	count = 1;
	flag = 0;
	if (n < 0 && (flag = 1))
		n *= -1;
	while (n >= 10)
	{
		n /= 10;
		count++;
	}
	return (count + flag);
}