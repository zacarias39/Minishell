/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 12:36:09 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:39 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_envars	*ft_realloc(void)
{
	ssize_t			i;
	char			**matrix;
	t_envars		*old_envars;
	static t_envars	new_envars;
	ssize_t			len;

	old_envars = get_envs(NULL);
	if (!old_envars)
		return (old_envars);
	matrix = old_envars->matrix;
	len = old_envars->capacity;
	new_envars.matrix = ft_calloc(len + MATRIX_SIZE + 1, sizeof(char *));
	if (!new_envars.matrix)
		return (old_envars);
	new_envars.n_items = 0;
	new_envars.capacity = len + MATRIX_SIZE;
	i = -1;
	while (++i < len)
	{
		if (matrix[i])
			new_envars.matrix[new_envars.n_items++] = matrix[i];
	}
	free(matrix);
	return (get_envs(&new_envars));
}

bool	ft_find_and_update(char *arg, t_envars *env_vars)
{
	size_t	len;
	ssize_t	i;

	i = -1;
	while (++i < env_vars->capacity)
	{
		if (!env_vars->matrix[i])
			continue ;
		len = strlen_chr(env_vars->matrix[i], '=');
		if (!ft_strncmp(env_vars->matrix[i], arg, len))
		{
			free(env_vars->matrix[i]);
			env_vars->matrix[i] = ft_strdup2(arg);
			return (true);
		}
	}
	return (false);
}

void	add_args(char *arg, t_envars *env_vars)
{
	ssize_t	i;

	i = -1;
	while (++i < env_vars->capacity)
	{
		if (!env_vars->matrix[i])
		{
			env_vars->matrix[i] = ft_strdup2(arg);
			env_vars->n_items++;
			break ;
		}
	}
}

void	update_env_vars(char **args)
{
	ssize_t		j;
	t_envars	*env_vars;

	j = -1;
	env_vars = get_envs(NULL);
	if (!env_vars)
		return ;
	while (args && args[++j])
	{
		if (!valid_identifier(args[j]))
			continue ;
		if (env_vars->n_items >= env_vars->capacity)
			env_vars = ft_realloc();
		if (!ft_find_and_update(args[j], env_vars))
			add_args(args[j], env_vars);
	}
	cmd_paths(true);
}

bool	ft_init_envars(char **envp, t_envars *new_envars)
{
	ssize_t	len;

	len = 0;
	while (envp && envp[len])
		len++;
	len += MATRIX_SIZE;
	new_envars->matrix = ft_calloc(len, sizeof(char *));
	if (!new_envars->matrix)
		return (false);
	new_envars->capacity = len;
	new_envars->n_items = 0;
	while (envp && *envp)
		new_envars->matrix[new_envars->n_items++] = ft_strdup2(*envp++);
	get_envs(new_envars);
	ft_update_shlvl();
	ft_update_pwd();
	cmd_paths(true);
	return (true);
}
