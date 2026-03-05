/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                              		:+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@42luanda.com     >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 08:50:49 by zcasimir         #+#    #+#              */
/*   Updated: 2025/06/17 08:50:49 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*node;

	if (!lst)
		return ;
	while (*lst)
	{
		node = (*lst)->next;
		if ((*lst)->content && del)
			del((*lst)->content);
		(*lst)->content = NULL;
		free(*lst);
		*lst = node;
	}
	*lst = NULL;
}
