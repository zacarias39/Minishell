/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:51:15 by zcasimir          #+#    #+#             */
/*   Updated: 2026/02/04 15:34:24 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	str_sub_len(char const *s1, char c)
{
	size_t	len;

	len = 0;
	while (s1[len] != c && s1[len])
		len++;
	return (len);
}

static size_t	word_count(char const *s, char c)
{
	size_t	word;
	size_t	count;

	count = 0;
	word = 0;
	while (*s)
	{
		if (*s == c)
		{
			word = 0;
		}
		else if (!word)
		{
			word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static char	**free_all(char **str, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		free(str[i++]);
	free(str);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	size_t	words;
	size_t	j;

	if (!s)
		return (NULL);
	words = word_count(s, c);
	str = (char **)ft_malloc((words + 1) * sizeof(char *), General);
	if (!str)
		return (NULL);
	j = 0;
	while (*s)
	{
		if (*s != c && j < words)
		{
			str[j] = ft_substr(s, 0, str_sub_len(s, c));
			if (!str[j++])
				return (free_all(str, j - 1));
			s += str_sub_len(s, c);
		}
		else
			s++;
	}
	return (str);
}
