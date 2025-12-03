/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:42:23 by zcasimir          #+#    #+#             */
/*   Updated: 2025/06/11 17:52:35 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arr;
	size_t	total;

	if (!size || !nmemb)
		return (malloc(0));
	total = size * nmemb;
	if ((total / nmemb) != size)
		return (NULL);
	arr = (void *)malloc(total);
	if (!arr)
		return (NULL);
	ft_bzero(arr, total);
	return (arr);
}
