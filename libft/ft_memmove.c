/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:39:30 by zcasimir          #+#    #+#             */
/*   Updated: 2025/06/11 09:31:14 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*s1;
	unsigned const char	*s2;

	if (!dest && !src)
		return (NULL);
	if (dest == src || n == 0)
		return (dest);
	s1 = dest;
	s2 = src;
	if (s1 > s2 && s1 <= s2 + n)
	{
		s1 += n;
		s2 += n;
		while (n--)
			*(--s1) = *(--s2);
	}
	else
		while (n--)
			*s1++ = *s2++;
	return (dest);
}
