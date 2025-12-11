/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:28:41 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/11 12:53:57 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

ssize_t	ft_partition(char ***arr, ssize_t low, ssize_t high)
{
	ssize_t	i;
	ssize_t	j;
	char	*pivot;

	j = low;
	i = low - 1;
	pivot = (*arr)[high - 1];
	while (j < high - 1)
	{
		if (ft_strcmp((*arr)[j], pivot) < 0)
		{
			i++;
			ft_swap((*arr) + i, (*arr) + j);
		}
		j++;
	}
	ft_swap((*arr) + (i + 1), (*arr) + (high - 1));
	return (i + 1);
}

void	quick_sort(char ***arr, ssize_t low, ssize_t high)
{
	ssize_t	pivot_index;

	if (low >= high)
		return ;
	pivot_index = ft_partition(arr, low, high);
	quick_sort(arr, low, pivot_index);
	quick_sort(arr, pivot_index + 1, high);
}

