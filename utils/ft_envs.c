/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 14:02:48 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/10 12:17:25 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*ft_getenv(const char *name)
{
	t_envars	*tmp_env;
	ssize_t		i;
	ssize_t		len;

	len = ft_strlen(name);
	i = -1;
	tmp_env = get_envs(NULL);
	if (!name || (!tmp_env || !tmp_env->matrix))
		return (NULL);
	while (++i < tmp_env->capacity)
	{
		if (!ft_strncmp(tmp_env->matrix[i], name, len))
		{
			if (!ft_strncmp(tmp_env->matrix[i] + len, "=", 1))
				return (tmp_env->matrix[i] + (++len));
		}
	}
	return (NULL);
}

char	**ft_realloc_matrix(char ***matrix, size_t *len)
{
	char	**new;
	char	**tmp;
	size_t	i;

	i = 0;
	*len += MATRIX_SIZE;
	tmp = *matrix;
	new = ft_malloc((*len + 1) * sizeof(char *), Tree);
	if (!new)
		return (NULL);
	while (*tmp)
		new[i++] = *tmp++;
	return (new);
}

char	**args_join(t_wordlist **end, char **matrix, char *token, size_t len)
{
	size_t		i;
	const char	*word = get_expansion(ft_strdup(token), Word);

	i = 0;
	while (true)
	{
		if (token && ft_strchr(QUOTES, *token) && !word)
			word = "";
		if (word && i >= len)
			matrix = ft_realloc_matrix(&matrix, &len);
		if (word)
			matrix[i++] = word;
		free(token);
		token = NULL;
		word = get_args(NULL, NEXT);
		if (!word && *end && (*end)->list[HEAD])
		{
			token = ft_strdup2((*end)->list[HEAD]->token);
			word = get_expansion((*end)->list[HEAD]->token, Word);
			(*end)->list[HEAD] = (*end)->list[HEAD]->next;
		}
		else if (!word)
			break ;
	}
	return (matrix);
}

char	**get_cmd_args(t_wordlist **list, char *cmd_name)
{
	size_t	len;
	char	**matrix;

	len = 0;
	if ((*list) && (*list)->list_len)
		len = (*list)->list_len;
	matrix = ft_malloc((len + 1) * sizeof(char *), Tree);
	if (!matrix)
		return (NULL);
	if (cmd_name)
		matrix = args_join(list, matrix, ft_strdup2(cmd_name), len);
	*list = NULL;
	return (matrix);
}
