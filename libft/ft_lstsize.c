/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                               		:+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@42luanda.com     >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:45:14 by zcasimir         #+#    #+#              */
/*   Updated: 2025/06/16 18:45:14 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		len;
	t_list	*node;

	len = 0;
	if (!lst)
		return (0);
	node = lst;
	while (node)
	{
		len++;
		node = node->next;
	}
	return (len);
}
