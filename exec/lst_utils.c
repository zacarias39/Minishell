/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 19:20:42 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 19:25:56 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "traversing_ast.h"

t_lst_fds	*ft_lstnew_fd(int fds[2])
{
	t_lst_fds	*node;

	node = (t_lst_fds *)malloc(sizeof(t_lst_fds));
	if (!node)
		return (NULL);
	node->fds[IN_FD] = fds[IN_FD];
	node->fds[OUT_FD] = fds[OUT_FD];
	node->next = NULL;
	return (node);
}

void	ft_lstadd_front_fd(t_lst_fds **lst, t_lst_fds *new)
{
	if (!new)
		return ;
	new->next = *lst;
	*lst = new;
}
