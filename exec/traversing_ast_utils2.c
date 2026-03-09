/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversing_ast_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 14:47:04 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 18:51:01 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "traversing_ast.h"

int	process_exit_status(int status, int *sig, int *sucess)
{
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status) + 128;
		if (*sucess == true && status == SIGQUIT_CODE)
			return (status);
		if (!*sig || (*sig && *sig != status))
		{
			*sig = status;
			handle_parent_sigint2(status - 128);
		}
	}
	else if (WIFSTOPPED(status))
		status = WSTOPSIG(status);
	return (status);
}

void	wait_child(void)
{
	int		sig;
	int		sucess;
	int		status;
	t_info	info;

	sig = 0;
	sucess = false;
	fork_calls_info(GET_LAST_PROC, NULL, &info.last_proc);
	fork_calls_info(GET_N_FORKS, NULL, &info.n_forks);
	while (info.n_forks > 0)
	{
		info.child_id = waitpid(-1, &status, WUNTRACED | WCONTINUED);
		status = process_exit_status(status, &sig, &sucess);
		if (status == EXIT_SUCCESS)
			sucess = true;
		if (info.child_id == info.last_proc)
			last_cmd_status(status, true);
		info.n_forks--;
	}
}

void	add_pipes_to_child(t_ast *head)
{
	head->right->fd_in = head->fds[PIPE_READ];
	head->left->fd_out = head->fds[PIPE_WRITE];
	if (head->fd_out != INVALID)
		head->right->fd_out = head->fd_out;
	if (head->fd_in != INVALID)
		head->left->fd_in = head->fd_in;
	if (head->left->type != Pipeline)
	{
		head->left->fds[PIPE_READ] = head->fds[PIPE_READ];
		head->left->fds[PIPE_WRITE] = head->fds[PIPE_WRITE];
	}
	if (head->right->type != Pipeline)
	{
		head->right->fds[PIPE_READ] = head->fds[PIPE_READ];
		head->right->fds[PIPE_WRITE] = head->fds[PIPE_WRITE];
	}
}

void	update_cmd_fds(t_ast *word_node)
{
	if (word_node->fd_in != INVALID)
	{
		dup2(word_node->fd_in, STDIN_FILENO);
		close(word_node->fd_in);
	}
	if (word_node->fd_out != INVALID)
	{
		dup2(word_node->fd_out, STDOUT_FILENO);
		close(word_node->fd_out);
	}
	if (word_node->fds[PIPE_READ] != INVALID)
		close(word_node->fds[PIPE_READ]);
	if (word_node->fds[PIPE_WRITE] != INVALID)
		close(word_node->fds[PIPE_WRITE]);
}

void	close_ast_node_fds(t_ast *head)
{
	if (head->fds[PIPE_READ] != INVALID)
		close(head->fds[PIPE_READ]);
	if (head->fds[PIPE_WRITE] != INVALID)
		close(head->fds[PIPE_WRITE]);
	if (head->fd_in != INVALID && head->fd_in != STDIN_FILENO)
		close(head->fd_in);
	if (head->fd_out != INVALID && head->fd_out != STDOUT_FILENO)
		close(head->fd_out);
}
