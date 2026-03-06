/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 12:55:38 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 13:57:33 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	special_paths(char *dir)
{
	size_t	i;
	int		dots;

	i = 0;
	dots = 0;
	while (dir[i])
	{
		if (dir[i] != '/' && dir[i] != '.')
			return (0);
		if (dir[i] == '.')
			dots++;
		if (dots > 2)
			return (0);
		i++;
	}
	return (dots);
}

void	update_path(char *new_dir)
{
	char	*tmp_dir;
	char	*tmp_dir2;
	int		status;

	status = last_cmd_status(NO_STATUS, GET_STATUS);
	if (status != EXIT_SUCCESS && status != GETCWD_ERROR)
		return ;
	if (status == EXIT_SUCCESS)
		trim(new_dir, ft_strlen(new_dir));
	tmp_dir = ft_strjoin2("OLDPWD=", ft_getenv("PWD"));
	tmp_dir2 = ft_strjoin2("PWD=", new_dir);
	get_current_dir(new_dir, true);
	update_env_vars((char *[2]){tmp_dir, NULL});
	update_env_vars((char *[2]){tmp_dir2, NULL});
	free(tmp_dir);
	free(tmp_dir2);
	if (status == GETCWD_ERROR)
		last_cmd_status(EXIT_SUCCESS, UPDATE_STATUS);
}

char	*change_directory(char *dir)
{
	ssize_t	i;
	bool	can_free;
	char	*tmp_dir;
	char	*aux;

	can_free = false;
	if (*dir == '/')
		return (last_cmd_status(chdir(dir), UPDATE_STATUS), dir);
	aux = get_current_dir(NULL, false);
	i = ft_strlen(aux);
	if (i > 0 && aux[i - 1] != '/')
	{
		aux = ft_strjoin2(aux, "/");
		can_free = true;
	}
	tmp_dir = ft_strjoin2(aux, dir);
	if (special_paths(dir) > 0)
		fallback_dots(&tmp_dir, dir);
	else
		last_cmd_status(chdir(tmp_dir), UPDATE_STATUS);
	if (can_free)
		free(aux);
	return (tmp_dir);
}

bool	get_cd_args(char **dir, char **paths)
{
	if (!paths)
		return (false);
	*dir = *paths;
	if (!*dir)
		return (ft_perror(MSH, "cd", CD_PATH_ERROR), false);
	else if (paths[0] && paths[1])
		return (ft_perror(MSH, "cd", TOO_MANY), false);
	return (true);
}

void	cd_cmd(char **path, t_ast *word)
{
	int		status;
	char	*dir;
	char	*new_dir;

	(void)word;
	if (!get_cd_args(&dir, path))
		return (last_cmd_status(EXIT_FAILURE, UPDATE_STATUS), (void)NULL);
	new_dir = change_directory(dir);
	status = last_cmd_status(NO_STATUS, GET_STATUS);
	if (status != EXIT_SUCCESS && status != GETCWD_ERROR)
	{
		ft_perror(MSH, NULL, NULL);
		ft_perror("cd", dir, "No such file or directory");
		last_cmd_status(EXIT_FAILURE, UPDATE_STATUS);
	}
	update_path(new_dir);
	if (new_dir != dir)
		free(new_dir);
}
