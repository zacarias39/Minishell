/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversing_ast_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 11:46:17 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 19:36:51 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "traversing_ast.h"

bool	check_and_create_pipe(t_ast *head, bool *from_fork)
{
	if (head->type == Pipeline)
	{
		if (pipe(head->fds) == INVALID)
			return (ft_perror(MSH, "pipe", strerror(errno)), false);
		add_pipes_to_child(head);
		ft_lstadd_front_fd(&head->fds_lst, ft_lstnew_fd(head->fds));
		if (head->left)
			head->left->fds_lst = head->fds_lst;
		if (head->right)
			head->right->fds_lst = head->fds_lst;
		*from_fork = true;
	}
	return (true);
}

bool	or_and_condition(t_ast *head)
{
	int	status;

	if (head->type == AndCondition)
	{
		wait_child();
		status = last_cmd_status(NO_STATUS, GET_STATUS);
		if (status != EXIT_SUCCESS)
			return (false);
	}
	else if (head->type == OrCondition)
	{
		wait_child();
		status = last_cmd_status(NO_STATUS, GET_STATUS);
		if (status == EXIT_SUCCESS || status == SIGINT_CODE)
			return (false);
	}
	return (true);
}

void	close_heredoc(t_ast *redir_node)
{
	t_word	*node;

	if (!redir_node)
		return ;
	node = *(redir_node->word->list);
	while (node)
	{
		if (node->type == Heredoc && node->fd != INVALID)
			close(node->fd);
		node = node->next;
	}
}
