/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 15:25:03 by zcasimir          #+#    #+#             */
/*   Updated: 2026/03/10 12:40:06 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_envars	*get_envs(t_envars *new)
{
	static t_envars	*envs;

	if (new)
		envs = new;
	return (envs);
}

char	*get_current_dir(char *dir, bool update)
{
	static char	*path;

	if (update)
	{
		free(path);
		path = ft_strdup2(dir);
	}
	return (path);
}

char	*get_status(void)
{
	return (ft_itoa(last_cmd_status(NO_STATUS, GET_STATUS)));
}

char	**get_envars_organized(void)
{
	ssize_t		i;
	ssize_t		j;
	t_envars	*envars;
	char		**matrix;
	char		*aux;

	i = 0;
	j = 0;
	envars = get_envs(NULL);
	if (envars == NULL || envars->capacity <= 0)
		return (NULL);
	matrix = ft_malloc((envars->n_items + 1) * sizeof(char *), Tree);
	if (!matrix)
		return (envars->matrix);
	while (i < envars->capacity)
	{
		aux = envars->matrix[i];
		if (aux && ft_strchr(aux, '='))
			matrix[j++] = envars->matrix[i];
		i++;
	}
	matrix[j] = NULL;
	return (matrix);
}
