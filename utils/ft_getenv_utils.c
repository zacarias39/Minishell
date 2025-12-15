/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 12:36:09 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/12 13:04:54y zcasimir         ###   ########.fr       */
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

size_t	strlen_chr(char *str, char limit)	
{
	size_t	len;	

	len = 0;
	while (str && str[len] && str[len] != limit)
		len++;
	return (len);
}

void	update_envars(t_envars *envars, char **envs)
{
	ssize_t	i;
	ssize_t	len;

	i = -1;
	len = 0;
	while (++i < envars->capacity)
	{
		// if we reached the end of the environment variable's array break ;
		if (envs[len] == NULL)
			break ;
		//if the last one is not NULL, then we know we are running out of space;
		if (envars->matrix[envars->capacity - 1])
			*envars = ft_realloc(envars);
		// if this env already exists, we free the old one and add the new one;
		if (!ft_strncmp(envars->matrix[i], envs[len], strlen_chr(envars->matrix[i], '=')))
		{
			free(envars->matrix[i]);
			envars->matrix[i] = ft_strdup(envs[len++]);
		}
		//if NULL then make the matrix[i] point to the newly environment variable;
		else if (envars->matrix[i] == NULL)
		{
			envars->matrix[i] = ft_strdup(envs[len++]);
			envars->n_items += 1;
		}
	}
	// sort it ;
	quick_sort(&envars->matrix, 0, envars->n_items);
	// update the function with the new environment variables;
	ft_getenv(envars, NULL);
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
		new_envars->matrix[new_envars->n_items++] = ft_strdup(*envp++);
	// sort it
	quick_sort(&new_envars->matrix, 0, new_envars->n_items);
	// update the function with the new environment variables;
	ft_getenv(new_envars, NULL);
	cmd_paths(true);
	return (true);
}
