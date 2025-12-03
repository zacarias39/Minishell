/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:24:28 by zcasimir          #+#    #+#             */
/*   Updated: 2025/06/11 17:06:33 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!big || !little)
		return (NULL);
	if (!(*little))
		return ((char *) big);
	i = 0;
	while (i < len && big[i])
	{
		j = 0;
		while (j + i < len && little[j] && big[i + j]
			&& little[j] == big[i + j])
			j++;
		if (little[j] == '\0')
			return ((char *) big + i);
		i++;
	}
	return (NULL);
}
