/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                  		:+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@42luanda.com     >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:49:28 by zcasimir         #+#    #+#              */
/*   Updated: 2025/06/14 15:49:28 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	nlen(long n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n *= -1;
		len++;
	}
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	ft_ascii(char *str, size_t len, long nb)
{
	if (nb < 0)
	{
		nb *= -1;
		str[0] = '-';
	}
	str[len] = '\0';
	while (nb)
	{
		str[--len] = nb % 10 + '0';
		nb /= 10;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	len;
	long	nb;

	nb = n;
	len = nlen(nb);
	str = (char *)ft_malloc((len + 1) * sizeof(char), General);
	if (!str)
		return (NULL);
	if (nb == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	ft_ascii(str, len, nb);
	return (str);
}
