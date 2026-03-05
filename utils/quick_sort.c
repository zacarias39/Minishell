/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:28:41 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:46 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	strcmp_case_insensitive(const char *s1, const char *s2)
{
	char	c1;
	char	c2;
	size_t	i;

	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (-1);
	if (!s2)
		return (1);
	i = 0;
	while (true)
	{
		c1 = ft_tolower(s1[i]);
		c2 = ft_tolower(s2[i]);
		if (!(c1 && c2 && (c1 == c2)))
			break ;
		i++;
	}
	return (c1 - c2);
}

void	ft_swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

ssize_t	ft_partition(char ***arr, ssize_t low, ssize_t high, int type)
{
	int		(*f)(const char *, const char *);
	ssize_t	i;
	ssize_t	j;
	char	*pivot;

	j = low;
	i = low - 1;
	pivot = (*arr)[high - 1];
	if (type == CASE_SENSITVE)
		f = ft_strcmp;
	else
		f = strcmp_case_insensitive;
	while (j < high)
	{
		if (f((*arr)[j], pivot) < 0)
			ft_swap((*arr) + ++i, (*arr) + j);
		j++;
	}
	ft_swap((*arr) + ++i, (*arr) + (high - 1));
	return (i);
}

void	quick_sort(char ***arr, ssize_t low, ssize_t high, int type)
{
	ssize_t	pivot_index;

	if (!(high - low > 1))
		return ;
	pivot_index = ft_partition(arr, low, high, type);
	quick_sort(arr, low, pivot_index, type);
	quick_sort(arr, pivot_index + 1, high, type);
}
