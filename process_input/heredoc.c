/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:06:11 by zcasimir          #+#    #+#             */
/*   Updated: 2026/03/04 16:43:38 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

int		g_sig_re;

size_t	remove_quotes(char *token)
{
	size_t	i;

	i = 0;
	while (token && token[i])
	{
		if (!quotes_del(&token[i]))
			i++;
	}
	return (i);
}

int	get_expr(char *prompt)
{
	char	*token;

	token = NULL;
	if (ft_strtok(NULL, PREVIOUS, NO))
		return (false);
	ft_strtok(NULL, NEXT, CLEAN);
	signal(SIGINT, handle_heredoc);
	while (g_sig_re != SIGINT && (token == NULL || !*token))
	{
		token = readline(prompt);
		if (!token && g_sig_re != SIGINT)
		{
			printf("mnsh: syntax error: unexpected end of file\n");
			// display the error message;
			exit(2);
		}
		if (!ft_strtok(token, NEXT, NO))
		{
			free(token);
			token = NULL;
		}
	}
	if (g_sig_re == SIGINT)
		return (set_default_stdin(STDIN_FILENO), false);
	return (parent_signal(IDLE), false);
}

void	get_line(char *delimeter, int fd, int line, int quotes)
{
	char	*str;
	double	count;

	str = NULL;
	count = 0;
	while (g_sig_re != SIGINT && ft_strcmp(str, delimeter))
	{
		if (str)
		{
			if (quotes == false)
				str = get_expansion(str, Heredoc);
			ft_putendl_fd(str, fd);
			count += 0.37;
		}
		str = readline("> ");
		if (!str && g_sig_re != SIGINT)
		{
			printf("mnsh: warning: here-document at line %d ", line
				+ (int)count);
			printf("delimited by end-of-file (wanted `%s')\n", delimeter);
			return ;
		}
	}
	free(str);
}

int	heredoc_count(int *fd, char op)
{
	static int	heredoc_len = 1;

	if (heredoc_len == 17)
	{
		ft_perror(MSH, NULL, "maximum here-document count exceeded");
		ft_free();	
		exit(1);
	}
	if (op == RESET)
		return (heredoc_len = 0, true);
	if (pipe(fd) == -1)
		return (false);
	heredoc_len++;
	collect_heredoc_fds(fd[PIPE_READ], UPDATE_DATA);
	fcntl(fd[PIPE_READ], F_SETFD, FD_CLOEXEC);
	fcntl(fd[PIPE_WRITE], F_SETFD, FD_CLOEXEC);
	return (true);
}

t_word	*get_heredoc(t_word *node, size_t token_len)
{
	static int	line;
	int			fd[2];

	if (++line && !node)
		return (NULL);
	signal(SIGINT, handle_heredoc);
	if (heredoc_count(fd, NO) == false)
		return (NULL);
	if (remove_quotes(node->token) < token_len)
		get_line(node->token, fd[PIPE_WRITE], line, true);
	else
		get_line(node->token, fd[PIPE_WRITE], line, false);
	close(fd[PIPE_WRITE]);
	node->fd = fd[PIPE_READ];
	if (g_sig_re == SIGINT)
	{
		set_default_stdin(STDIN_FILENO);
		close(fd[PIPE_READ]);
		node = NULL;
	}
	parent_signal(IDLE);
	return (node);
}
