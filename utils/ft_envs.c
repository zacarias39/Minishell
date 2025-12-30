/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 14:02:48 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/12 00:42:19 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(t_envars *envars, const char *name)
{
	static char	**tmp_env;
	size_t		i;
	size_t		len;

	len = ft_strlen(name);
	i = 0;
	if (envars)
		tmp_env = envars->matrix;
	if (!name)
		return (NULL);
	while (tmp_env[i])
	{
		if (!ft_strncmp(tmp_env[i], name, len))
		{
			if (!ft_strncmp(tmp_env[i] + len, "=", 1))
				return (tmp_env[i] + (++len));	
		}
		i++;
	}
	return (NULL);
}

char	**matrix_from_list(t_wordlist **list, char *cmd_name)
{
	t_word	*node;
	size_t	i;
	size_t	len;
	char	**matrix;

	len = 1;
	node = NULL; 
	if ((*list) && (*list)->list_len) {
		len += (*list)->list_len;
		node = (*list)->list[HEAD];
	}
	matrix = malloc(sizeof(char *) * (len + 1));
	i = 0;
	if (!matrix) // ERROR
		return (NULL); // CHANGE IT LATER
	matrix[i++] = cmd_name;
	while (node)
	{
		// duplicating the string is need here, because those string inside the list can't be freed;
		matrix[i++] = node->token;
		(*list)->list[HEAD] = node;
		node = node->next;
		free((*list)->list[HEAD]);
	}
	matrix[i] = NULL;
	free(*list);
	*list = NULL;
	return (matrix);
}
