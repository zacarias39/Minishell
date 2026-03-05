/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 14:02:48 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:38 by zcasimir        ###   ########.fr        */
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

char	**add_to_args(t_wordlist **list, char **matrix, char *cmd_name)
{
	size_t	i;
	t_word	*node;
	char	*word;

	i = 0;
	node = NULL;
	if (*list)
		node = (*list)->list[HEAD];
	word = get_expansion(cmd_name, Word);
	while (word)
	{
		if (word)
			matrix[i++] = word;
		else
			break ;
		word = get_args(NULL, NEXT);
		if (!word && node)
		{
			word = get_expansion(node->token, Word);
			node = node->next;
		}
	}
	*list = NULL;
	return (matrix);
}

char	**matrix_from_list(t_wordlist **list, char *cmd_name)
{
	size_t	len;
	ssize_t	i;
	char	**matrix;

	i = 0;
	len = 1;
	if ((*list) && (*list)->list_len)
		len += (*list)->list_len;
	matrix = ft_malloc(sizeof(char *) * (len + 1), Tree);
	if (!matrix)
		return (NULL);
	matrix = add_to_args(list, matrix, cmd_name);
	return (matrix);
}
