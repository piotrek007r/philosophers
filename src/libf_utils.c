#include "../include/philosophers.h"

void	ft_putstr_fd(char *s, int fd)
{
	unsigned int	counter;

	counter = 0;
	while (s[counter] != '\0')
	{
		write(fd, &s[counter], 1);
		counter++;
	}
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
	return ;
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		ft_putchar_fd('-', fd);
		ft_putchar_fd('2', fd);
		n = 147483648;
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd(n % 10 + '0', fd);
	}
	else
		ft_putchar_fd(n + '0', fd);
}

int	ft_atoi(const char *str)
{
	int	isminus;
	int	result;

	isminus = 1;
	result = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			isminus = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = (result * 10) + *str - '0';
		str++;
	}
	return (result * isminus);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*temp;
	size_t			counter;

	if (size == 0 || nmemb == 0)
	{
		size = 1;
		nmemb = 1;
	}
	if (size > 2147483647 / nmemb)
	{
		return (NULL);
	}
	temp = (void *)malloc(nmemb * size);
	if (!temp)
		return (NULL);
	counter = 0;
	while ((nmemb * size) > counter)
	{
		temp[counter] = 0;
		counter++;
	}
	return (temp);
}