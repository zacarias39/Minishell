/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadivaldo <dadivaldo@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 10:25:52 by dadivaldo         #+#    #+#             */
/*   Updated: 2025/12/04 11:12:17 by dadivaldo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_free_t_envp(t_envp *env_metadata)
{
	size_t	i;

	i = 0;
	while (i < env_metadata->capacity)
	{
		free(env_metadata->matrix[i]);
		i++;
	}
	free(env_metadata->matrix);
	env_metadata->matrix = NULL;
	env_metadata->top = 0;
	env_metadata->capacity = 0;
}

void	ft_copy_to(t_envp *new_matrix, t_envp *old_matrix) // Dup2
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!new_matrix || !old_matrix)
		return ;
	if (new_matrix->capacity < old_matrix->capacity)
		return ;
	while (i < old_matrix->capacity)
	{
		if (old_matrix->matrix[i])
			new_matrix->matrix[j++] = ft_strdup(old_matrix->matrix[i]);
		i++;
	}
	new_matrix->top = j;
}

char	*ft_getenv(t_envp env_metadata, const char *name) // AlphaNuum only
{
	size_t	i;
	size_t	name_len;

	i = 0;
	name_len = ft_strlen(name);
	while (i < env_metadata.capacity)
	{
		if (!ft_strncmp(env_metadata.matrix[i], name, name_len))
			return (env_metadata.matrix[i] + name_len);
		i++;
	}
	return (NULL);
}

t_envp	create_env(size_t len)
{
	t_envp	env_metadata;
	char	**args;

	args = calloc(len, sizeof(char *));
	env_metadata.matrix = args;
	env_metadata.capacity = len;
	env_metadata.top = 0;
	return (env_metadata);
}

t_envp	ft_initialize(char **envs)
{
	size_t	i;
	t_envp	env_metadata;

	i = 0;
	while (envs[i])
		i++;
	env_metadata = create_env(i + 1);
	ft_copy_to(&env_metadata, &(t_envp){envs, 0, i});
	return (env_metadata);
}
