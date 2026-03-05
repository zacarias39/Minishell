/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:45:13 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:50:29 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	set_default_stdin(int fd)
{
	static int	stdin_backup;

	if (fd != STDIN_FILENO && fd >= 0)
	{
		stdin_backup = fd;
		return (0);
	}
	else if (fd == -3)
		return (stdin_backup);
	else if (fd == CLOSE)
	{
		close(stdin_backup);
		return (0);
	}
	if (fd < 0)
		return (0);
	dup2(stdin_backup, STDIN_FILENO);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_ast		*root;
	t_envars	envars_info;

	(void)ac;
	(void)av;
	ft_init_envars(envp, &envars_info);
	set_default_stdin(dup(STDIN_FILENO));
	while (true)
	{
		parent_signal(IDLE);
		line = get_user_input();
		if (!line)
			exit_cmd(NULL, STDOUT_FILENO);
		root = NULL;
		if (ft_strtok(line, NEXT, NO))
			root = parse_expression();
		parent_signal(BUSY);
		traversing_ast(root, false);
		wait_child();
		ft_malloc(0, TreeFree);
		fork_calls_info(RESET_DATA, NULL, NULL);
	}
	rl_clear_history();
}
