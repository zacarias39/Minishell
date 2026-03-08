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

bool	is_valid_aux(char **n, char **p, char **m, const char **s)
{
	if (**n == **p)
	{
		*n = *n + 1;
		*p = *p + 1;
	}
	else if (**p == '*')
	{
		*s = *p;
		*m = *n;
		*p = *p + 1;
	}
	else
		return (false);
	return (true);
}

bool	is_valid(char *name, char *pattern)
{
	char		*n_match;
	const char	*star_pos = NULL;

	if (*name == '.')
		return (false);
	while (*name)
	{
		if (is_valid_aux(&name, &pattern, &n_match, &star_pos))
			continue ;
		else if (!star_pos)
			return (false);
		else
		{
			pattern = (char *)star_pos + 1;
			name = ++n_match;
		}
	}
	while (*pattern)
	{
		if (*pattern != '*')
			return (false);
		pattern++;
	}
	return (true);
}

ssize_t	count_dir_files(DIR *dir, char *pattern)
{
	struct dirent	*file_dir;
	ssize_t			i;

	i = 0;
	while (1)
	{
		file_dir = readdir(dir);
		if (!file_dir)
			break ;
		if (!is_valid(file_dir->d_name, pattern))
			continue ;
		i++;
	}
	return (i);
}

void	copy_names_to_arr(DIR *dir, char **matrix, char *pattern)
{
	struct dirent	*file_dir;
	ssize_t			i;

	i = 0;
	while (1)
	{
		file_dir = readdir(dir);
		if (!file_dir)
			break ;
		if (!is_valid(file_dir->d_name, pattern))
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
