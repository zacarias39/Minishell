/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_wildcard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 16:43:39 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:43 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

bool	is_valid(char *name, ssize_t n_len, char *pattern, ssize_t p_len)
{
	ssize_t	n_index;
	ssize_t	p_index;
	ssize_t	n_match;
	ssize_t	star_index;

	n_index = 0;
	p_index = 0;
	star_index = INVALID;
	if (*name == '.')
		return (false);
	while (n_index < n_len)
	{
		if (name[n_index] == pattern[p_index])
		{
			n_index++;
			p_index++;
		}
		else if (pattern[p_index] == '*')
		{
			star_index = p_index++;
			n_match = n_index;
		}
		else if (star_index == INVALID)
			return (false);
		else
		{
			p_index = star_index + 1;
			n_index = ++n_match;
		}
	}
	while (p_index < p_len)
	{
		if (pattern[p_index] != '*')
			return (false);
		p_index++;
	}
	return (true);
}

ssize_t	count_dir_files(DIR *dir, char *pattern)
{
	struct dirent	*file_dir;
	const ssize_t	pattern_len = ft_strlen(pattern);
	ssize_t			i;

	i = 0;
	while (1)
	{
		file_dir = readdir(dir);
		if (!file_dir)
			break ;
		if (!is_valid(file_dir->d_name, ft_strlen(file_dir->d_name), pattern, pattern_len))
			continue ;
		i++;
	}
	return (i);
}

void	copy_names_to_arr(DIR *dir, char **matrix, char *pattern)
{
	struct dirent	*file_dir;
	const ssize_t	pattern_len = ft_strlen(pattern);
	ssize_t			i;

	i = 0;
	while (1)
	{
		file_dir = readdir(dir);
		if (!file_dir)
			break ;
		if (!is_valid(file_dir->d_name, ft_strlen(file_dir->d_name), pattern, pattern_len))
			continue ;
		matrix[i++] = ft_strdup(file_dir->d_name);
	}
	matrix[i] = NULL;
}

t_list	*get_dir_datas(char *path)
{
	char	**arr;
	t_list	*head;
	ssize_t	i;
	DIR		*dir;

	i = 0;
	head = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	i = count_dir_files(dir, path);
	arr = malloc(sizeof(char *) * (i + 1));
	if (!arr)
		return (closedir(dir), NULL);
	closedir(dir);
	dir = opendir(".");
	if (!dir)
		return (NULL);
	copy_names_to_arr(dir, arr, path);
	closedir(dir);
	quick_sort(&arr, 0, i, CASE_INSENSITVE);
	i = -1;
	while (arr[++i])
		ft_lstadd_back(&head, ft_lstnew(arr[i]));
	return (free(arr), head);
}
