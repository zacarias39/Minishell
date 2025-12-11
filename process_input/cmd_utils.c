/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:08:15 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/11 12:38:51 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	**cmd_paths(bool update)
{
	static char	**matrix;

	if (update)
	{
		ft_free_matrix(&matrix);
		matrix = ft_split(ft_getenv(NULL, "PATH"), ':');
	}
	return (matrix);
}

char	*get_cmd_path(char *cmd)
{
	char	**paths;
	char	*str_2;
	char	*aux;
	size_t	i;

	paths = cmd_paths(false);
	if (ft_strchr(cmd, '/'))
		return (cmd);
	i = 0;
	aux = ft_strjoin("/", cmd);
	while (paths && paths[i])
	{
		str_2 = ft_strjoin(paths[i++], aux);
		if (!access(str_2, F_OK))
		{
			free(aux);
			return (str_2);
		}
		free(str_2);
	}
	free(aux);
	return (NULL);
}
