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

int	set_default_std_fd(int fd, int backup)
{
	static int	stdin_backup;
	static int	stdout_backup;
	static int	stderr_backup;

	if (backup == CLOSE)
	{
		close(stdin_backup);
		close(stdout_backup);
		return (close(stderr_backup));
	}
	else if (backup == STDIN_FILENO && fd > 2)
		return (stdin_backup = fd, fd);
	else if (backup == STDOUT_FILENO && fd > 2)
		return (stdout_backup = fd, fd);
	else if (backup == STDERR_FILENO && fd > 2)
		return (stderr_backup = fd, fd);
	else if (fd == INVALID)
		return (INVALID);
	if (backup == STDIN_FILENO)
		return (dup2(stdin_backup, STDIN_FILENO));
	else if (backup == STDOUT_FILENO)
		return (dup2(stdout_backup, STDOUT_FILENO));
	return (dup2(stderr_backup, STDOUT_FILENO));
}

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_ast		*root;
	t_envars	envars_info;

	(void)ac;
	(void)av;
	ft_init_envars(envp, &envars_info);
	while (true)
	{
		parent_signal(IDLE);
		line = get_user_input();
		if (!line)
			exit_cmd(NULL, NULL);
		root = NULL;
		parent_signal(BUSY);
		if (ft_strtok(line, NEXT, NO))
			root = parse_expression();
		if (root == NULL)
			continue ;
		traversing_ast(root, false);
		wait_child();
		ft_malloc(0, TreeFree);
		fork_calls_info(RESET_DATA, NULL, NULL);
	}
}
