/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:32:00 by zcasimir          #+#    #+#             */
/*   Updated: 2025/06/11 13:47:41 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*chr;

	chr = NULL;
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (unsigned char) c)
			chr = (char *) s;
		s++;
	}
	if ((unsigned char) c == '\0')
		return ((char *) s);
	return ((char *) chr);
}
