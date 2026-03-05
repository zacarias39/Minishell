/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 11:46:32 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:33 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

// ft_free_matrix(&matrix);
char	**cmd_paths(bool update)
{
	static char	**matrix;

	if (update)
	{
		matrix = ft_split(ft_getenv("PATH"), ':');
	}
	return (matrix);
}

bool	is_dir(char *cmd)
{
	struct stat	file_status;

	if (stat(cmd, &file_status))
		return (false);
	if (S_ISDIR(file_status.st_mode))
	{
		ft_perror(MSH, cmd, "Is a directory");
		last_cmd_status(126, UPDATE_DATA);
		return (true);
	}
	return (false);
}

// Cmd check error
char	*ft_cmd_error(char *cmd, char *cmd_name, bool is_absolute)
{
	if (!cmd || access(cmd, F_OK))
	{
		if (is_absolute)
			ft_perror(MSH, cmd_name, "No such file or directory");
		else
			ft_perror(NULL, cmd_name, "command not found");
		if (is_absolute == false)
			free(cmd);
		last_cmd_status(Notfound, true);
		return (NULL);
	}
	if (access(cmd, X_OK))
	{
		ft_perror(MSH, cmd_name, "Permission denied");
		if (is_absolute == false)
			free(cmd);
		last_cmd_status(Denied, true);
		return (NULL);
	}
	if (is_dir(cmd_name))
		return (NULL);
	if (is_absolute)
		return (ft_strdup(cmd));
	return (cmd);
}
