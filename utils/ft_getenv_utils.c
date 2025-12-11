/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 12:36:09 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/11 13:31:48 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_free_envars(t_envars *old_envars)
{
	ssize_t	i;

	i = 0;
	while (i < old_envars->capacity)
	{
		if (old_envars->matrix[i])
			free(old_envars->matrix[i]);
		i++;
	}
	free(old_envars->matrix);
	old_envars->capacity = 0;
	old_envars->n_items = 0;
}

t_envars	ft_realloc(t_envars *old_envars)
{
	t_envars	new_envars;
	ssize_t		len;

	len = old_envars->capacity;
	new_envars.matrix = ft_calloc(len + MATRIX_SIZE, sizeof(char *));
	if (!new_envars.matrix)
		return (*old_envars);
	new_envars.n_items = 0;
	new_envars.capacity = old_envars->capacity + MATRIX_SIZE;
	while (len--)
	{
		if (old_envars->matrix[len])
			new_envars.matrix[new_envars.n_items++] = old_envars->matrix[len];
	}
	ft_free_envars(old_envars);
	return (new_envars);
}

void	update_envars(t_envars *envars, char **envs)
{
	ssize_t	i;
	ssize_t	len;

	i = -1;
	len = 0;
	while (++i < envars->capacity)
	{
		//if the last one is not NULL, then we know we are running out of space;
		if (envars->matrix[envars->capacity - 1])
			*envars = ft_realloc(envars);
		//if NULL then make the matrix[i] point to the newly environment variable;
		if (envars->matrix[i] == NULL)
		{
			// if we reached the end of the environment variable's array break ;
			if (envs[len] == NULL)
				break ;
			envars->matrix[i] = ft_strdup(envs[len++]);
			envars->n_items += 1;
}
	}
	// update the function with the new environment variables;
	ft_getenv(envars, NULL);
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
		new_envars->matrix[new_envars->n_items++] = ft_strdup(*envp++);
	ft_getenv(new_envars, NULL);
	return (true);
}
