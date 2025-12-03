/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:57:25 by zcasimir          #+#    #+#             */
/*   Updated: 2025/06/10 15:38:43 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*str1;
	unsigned const char	*str2;

	if (!dest && !src && n == 0)
		return (NULL);
	if (!dest || !src)
		return (dest);
	str1 = dest;
	str2 = src;
	while (n--)
	{
		*str1 = *str2;
		str1++;
		str2++;
	}
	return (dest);
}
