/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:33:26 by zcasimir          #+#    #+#             */
/*   Updated: 2025/06/10 12:12:34 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;

	if (!s && n > 0)
		return (NULL);
	if (n == 0)
		return (s);
	str = s;
	while (n--)
		str[n] = (unsigned char) c;
	return (s);
}
