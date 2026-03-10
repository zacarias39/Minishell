/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:02:33 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 13:57:33 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	print_cd_error(void)
{
	ft_perror(MSH, "cd", NULL);
	ft_putstr_fd("error retrieving current directory:", STDERR_FILENO);
	ft_putstr_fd(" getcwd: cannot access parent directories: ", STDERR_FILENO);
	ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
}

bool	check_and_restore_logical_path(char *target, char *new_dir)
{
	struct stat	current_s;
	const char	*current = get_current_dir(NULL, false);
	struct stat	target_s;

	if (lstat(current, &current_s) == INVALID)
		return (false);
	if (!S_ISLNK(current_s.st_mode))
		return (false);
	if (stat(target, &target_s) == INVALID)
	{
		trim(new_dir, ft_strlen(new_dir));
		last_cmd_status(chdir(new_dir), true);
		return (true);
	}
	stat(current, &current_s);
	if (current_s.st_ino == target_s.st_ino)
	{
		if (current_s.st_dev != target_s.st_dev)
			return (false);
		trim(new_dir, ft_strlen(new_dir));
		last_cmd_status(chdir(new_dir), true);
		return (true);
	}
	return (false);
}

int	try_change_dir(char **tmp_dir, char *user_dir_input)
{
	int	tries;

	tries = 1;
	*tmp_dir = getcwd(NULL, 0);
	chdir(user_dir_input);
	if (!*tmp_dir)
	{
		*tmp_dir = getcwd(NULL, 0);
		if (!*tmp_dir)
		{
			last_cmd_status(GETCWD_ERROR, UPDATE_STATUS);
			print_cd_error();
			return (tries);
		}
		tries = 2;
	}
	last_cmd_status(EXIT_SUCCESS, UPDATE_STATUS);
	return (tries);
}

void	fallback_dots(char **new_dir, char *dir)
{
	char	*tmp_dir;
	bool	second_try;
	char	*aux;

	aux = NULL;
	second_try = false;
	if (try_change_dir(&tmp_dir, dir) > 1)
		second_try = true;
	if (!check_and_restore_logical_path(tmp_dir, *new_dir))
	{
		if (!tmp_dir)
			return ;
		free(*new_dir);
		if (second_try)
			return (*new_dir = tmp_dir, (void) NULL);
		aux = ft_strjoin2("/", dir);
		*new_dir = ft_strjoin2(tmp_dir, aux);
		free(aux);
		free(tmp_dir);
		return ;
	}
	free(tmp_dir);
}
