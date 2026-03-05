/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:08:15 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:30 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*get_cmd_path(char *cmd)
{
	char	**paths;
	char	*str_2;
	char	*aux;
	size_t	i;

	i = 0;
	paths = cmd_paths(false);
	if (!cmd || !*cmd)
		return (false);
	if (!paths || ft_strchr(cmd, '/'))
		return (ft_cmd_error(cmd, cmd, true));
	aux = ft_strjoin("/", cmd);
	while (paths && paths[i])
	{
		str_2 = ft_strjoin(paths[i++], aux);
		if (!access(str_2, F_OK))
			break ;
		str_2 = NULL;
	}
	return (ft_cmd_error(str_2, cmd, false));
}

bool	create_fork(void)
{
	pid_t	id;

	id = fork();
	if (id == 0)
		return (true);
	if (id == -1)
	{
		last_cmd_status(254, UPDATE_DATA);
		ft_perror(MSH, "fork", strerror(errno));
		return (false);
	}
	fork_calls_info(UPDATE_DATA, &id, NULL);
	return (false);
}

void	fork_calls_info(uint8_t type, pid_t *id, ssize_t *return_data)
{
	static pid_t	last_proc = -1;
	static ssize_t	n_of_procs = 0;

	if (id && *id && type == UPDATE_DATA)
	{
		n_of_procs++;
		last_proc = *id;
	}
	if (type == RESET_DATA)
	{
		last_proc = -1;
		n_of_procs = 0;
	}
	if (return_data && type == GET_LAST_PROC)
		*return_data = last_proc;
	else if (return_data && type == GET_N_FORKS)
		*return_data = n_of_procs;
}

int	last_cmd_status(int status, bool update)
{
	static uint8_t	last_status = 0;

	if (update)
		last_status = status;
	return (last_status);
}
