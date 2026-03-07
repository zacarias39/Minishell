/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversing_ast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 13:52:06 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 19:38:41 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "traversing_ast.h"

/* putting root->word on a condition is pointless and
	can result in unwanted behaviour
 because it will always be NULL,
	as it's being freed inside maktrix_from_list function
 that's way I removed it;
 OBS: We must free the line returned by the readline
 */

bool	builtin_cmd(t_ast *word, t_ast *redir_node, bool from_fork)
{
	t_builtin	builtin;

	if (!word)
		return (false);
	builtin = get_builtin_info(word);
	if (builtin.error)
		return (false);
	if (word->fd_out == INVALID)
		word->fd_out = STDOUT_FILENO;
	if (builtin.redir(redir_node, &word->fd_in, &word->fd_out, from_fork))
		builtin.cmd_exec(builtin.args, word);
	if (!from_fork)
		return (true);
	close_fds(redir_node, word);
	ft_free();
	exit(last_cmd_status(NO_STATUS, GET_STATUS));
}

void	ft_execute_cmd(t_ast *word, t_ast *redir_node, bool from_fork)
{
	char	*cmd_path;

	if (from_fork && !create_fork())
		return ;
	word->args_token = matrix_from_list(&word->word, word->token);
	if (builtin_cmd(word, redir_node, from_fork))
		return ;
	if (!from_fork && !create_fork())
		return ;
	if (!ft_redirlist(redir_node, &word->fd_in, &word->fd_out, from_fork))
		exit((ft_free(), last_cmd_status(NO_STATUS, false)));
	update_cmd_fds(word);
	cmd_path = get_cmd_path(word->args_token[0]);
	if (cmd_path)
	{
		get_envars_organized();
		execve(cmd_path, word->args_token, get_envs(NULL)->matrix);
		ft_perror(MSH, "execve", strerror(errno));
		last_cmd_status(126, UPDATE_DATA);
	}
	collect_heredoc_fds(NO, CLOSE);
	close_fds(redir_node, word);
	ft_free();
	exit(last_cmd_status(NO_STATUS, false));
}

void	traversing_command(t_ast *node, bool from_fork)
{
	t_ast	*word_node;
	t_ast	*redir_node;

	if (!node)
		return ;
	if (node->left->type == RedirList)
	{
		redir_node = node->left;
		word_node = node->right;
	}
	else
	{
		redir_node = node->right;
		word_node = node->left;
	}
	word_node->fd_in = node->fd_in;
	word_node->fd_out = node->fd_out;
	word_node->fds[PIPE_READ] = node->fds[PIPE_READ];
	word_node->fds[PIPE_WRITE] = node->fds[PIPE_WRITE];
	word_node->fds_lst = node->fds_lst;
	ft_execute_cmd(word_node, redir_node, from_fork);
	return (close_fds(redir_node, word_node));
}

void	traversing_paren_expression(t_ast *head)
{
	if (!create_fork())
		return ;
	head->paren = false;
	if (head->fd_in != INVALID)
	{
		dup2(head->fd_in, STDIN_FILENO);
		close(head->fd_in);
	}
	if (head->fd_out != INVALID)
	{
		dup2(head->fd_out, STDOUT_FILENO);
		close(head->fd_out);
	}
	if (head->fds[PIPE_READ] != INVALID)
		close(head->fds[PIPE_READ]);
	if (head->fds[PIPE_WRITE] != INVALID)
		close(head->fds[PIPE_WRITE]);
	traversing_ast(head, false);
	wait_child();
	ft_free();
	exit(last_cmd_status(NO_STATUS, GET_STATUS));
}

void	traversing_ast(t_ast *head, bool from_fork)
{
	if (!head)
		return ;
	if (head->paren)
		return (traversing_paren_expression(head));
	if (head->type == Command)
		return (traversing_command(head, from_fork), (void) NULL);
	if (head->type == RedirList)
	{
		ft_redirlist(head, NULL, NULL, from_fork);	
		return (close_fds(head, NULL));
	}
	if (head->type == Word)
		return (ft_execute_cmd(head, NULL, from_fork), (void) NULL);
	if (!check_and_create_pipe(head, &from_fork))
		return ;
	traversing_ast(head->left, from_fork);
	if (!or_and_condition(head))
		return ;
	traversing_ast(head->right, from_fork);
	close_ast_node_fds(head);
	return ;
}
