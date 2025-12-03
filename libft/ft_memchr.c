/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:34:40 by zcasimir          #+#    #+#             */
/*   Updated: 2025/06/11 15:42:33 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned const char	*str;
	unsigned char		chr;

	if (!s && n > 0)
		return (NULL);
	str = s;
	chr = c;
	while (n--)
	{
		if (*str == chr)
			return ((void *) str);
		str++;
	}
	return (NULL);
}
