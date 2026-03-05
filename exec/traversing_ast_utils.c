/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversing_ast_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:57:41 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:54:07 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "traversing_ast.h"

bool	check_file(char *file_name, t_token_type type)
{
	if (type == RedirLeft)
	{
		if ((!file_name || access(file_name, F_OK)))
		{
			ft_perror(MSH, file_name, "No such file or directory");
			last_cmd_status(REDIR_ERROR, true);
			return (false);
		}
		if (access(file_name, R_OK))
		{
			ft_perror(MSH, file_name, "Permission denied");
			last_cmd_status(REDIR_ERROR, true);
			return (false);
		}
	}
	else if (!access(file_name, F_OK) && access(file_name, W_OK))
	{
		ft_perror(MSH, file_name, "Permission denied");
		last_cmd_status(REDIR_ERROR, true);
		return (false);
	}
	return (true);
}

static inline bool	get_outfile(t_word *outfile, int *old_out_fd)
{
	char	*filename;

	if (!(outfile->type == RedirRight || outfile->type == Append))
		return (true);
	filename = get_expansion(outfile->token, RedirList);
	if (!filename)
		return (false);
	if (!check_file(filename, outfile->type))
		return (*old_out_fd = INVALID, false);
	if (outfile->type == RedirRight)
		*old_out_fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0772);
	else
		*old_out_fd = open(filename, O_CREAT | O_WRONLY | O_APPEND);
	outfile->fd = *old_out_fd;
	if (*old_out_fd == INVALID)
		return (ft_perror(MSH, "open", strerror(errno)), false);
	return (true);
}

static inline bool	get_infile(t_word *infile, int *old_in_fd)
{
	char	*filename;

	if (!(infile->type == RedirLeft || infile->type == Heredoc))
		return (true);
	if (infile->type == Heredoc)
		return (*old_in_fd = infile->fd, true);
	filename = get_expansion(infile->token, RedirList);
	if (!filename)
		return (false);
	if (!check_file(filename, infile->type))
		return (*old_in_fd = INVALID, false);
	*old_in_fd = open(filename, O_RDONLY);
	infile->fd = *old_in_fd;
	if (*old_in_fd == INVALID)
		return (ft_perror(MSH, "open", strerror(errno)), false);
	return (true);
}

bool	ft_redirlist(t_ast *head, int *input, int *output)
{
	t_word	*node;
	int		fd[2];

	fd[IN_FD] = -1;
	fd[OUT_FD] = -1;
	if (!head)
		return (true);
	node = *head->word->list;
	while (node)
	{
		if (!get_infile(node, &fd[IN_FD]))
			return (close_fds(head, NULL), false);
		else if (!get_outfile(node, &fd[OUT_FD]))
			return (close_fds(head, NULL), false);
		node = node->next;
	}
	if (fd[IN_FD] != INVALID && input)
		*input = fd[IN_FD];
	else if (fd[IN_FD] != INVALID)
		close(fd[IN_FD]);
	if (fd[OUT_FD] != INVALID && output)
		*output = fd[OUT_FD];
	else if (fd[OUT_FD] != INVALID)
		close(fd[OUT_FD]);
	return (true);
}

t_builtin	get_builtin_info(t_ast *node, t_ast *redir)
{
	t_builtin	builtin;

	builtin.redir = ft_redirlist;
	builtin.fd_in = &node->fd_in;
	builtin.fd_out = &node->fd_out;
	builtin.args = (node->args_token) + JMP_CMD_NAME;
	if (!ft_strcmp(node->token, "echo"))
		builtin.cmd_exec = echo_cmd;
	else if (!ft_strcmp(node->token, "cd"))
		builtin.cmd_exec = cd_cmd;
	else if (!ft_strcmp(node->token, "export"))
		builtin.cmd_exec = export_cmd;
	else if (!ft_strcmp(node->token, "pwd"))
		builtin.cmd_exec = pwd_cmd;
	else if (!ft_strcmp(node->token, "unset"))
		builtin.cmd_exec = unset_cmd;
	else if (!ft_strcmp(node->token, "exit"))
	{
		close_fds(redir, node);
		builtin.cmd_exec = exit_cmd;
	}
	else if (!ft_strcmp(node->token, "env"))
		builtin.cmd_exec = env_cmd;
	else
		return (builtin.error = true, builtin);
	return (builtin.error = false, builtin);
}
