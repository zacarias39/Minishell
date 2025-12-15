/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:08:15 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/16 00:02:10 by zcasimir         ###   ########.fr       */
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

char	*ft_cmd_error(char *cmd, char *cmd_name, bool is_absolute)
{
	// checks if the cmd exists
    if (!cmd || access(cmd, F_OK))
    {
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		// checks if the user has given the absolute path for the cmd
        if (is_absolute)
		    ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        else
            ft_putendl_fd(": command not found", STDERR_FILENO);
        if (is_absolute == false)
            free(cmd);
		return (NULL);
    }
	// checks if the cmd has executable permission
    if (access(cmd, X_OK))
    {
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
        if (is_absolute == false)
            free(cmd);
		return (NULL);
    }
    if (is_absolute)
        return (ft_strdup(cmd));
    return (cmd);
}

char	*get_cmd_path(char *cmd)
{
	char	**paths;
	char	*str_2;
	char	*aux;
	size_t	i;

	i = 0;
	paths = cmd_paths(false);
	// checking if the paths is NULL or if the user has given the absolute path
	if (!paths || ft_strchr(cmd, '/'))
		return (ft_cmd_error(cmd, cmd, true));
	aux = ft_strjoin("/", cmd);
	while (paths && paths[i])
	{
		str_2 = ft_strjoin(paths[i++], aux);
		if (!access(str_2, F_OK))
			break ;
		free(str_2);
		str_2 = NULL;
	}
	free(aux);
	return (ft_cmd_error(str_2, cmd, false));
}
