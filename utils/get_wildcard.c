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

/*
 * Fluxograma
 * opendir -> count_dir files -> allocate the array, dup and store it, sort,
	then return
 * add dir on list -> add on the list of
 *
 */

ssize_t	count_dir_files(DIR *dir)
{
	struct dirent	*file_dir;
	ssize_t			i;

	i = 0;
	while (1)
	{
		file_dir = readdir(dir);
		if (!file_dir)
			break ;
		if (file_dir && file_dir->d_name[0] == '.')
			continue ;
		i++;
	}
	return (i);
}

void	copy_names_to_arr(DIR *dir, char **matrix)
{
	struct dirent	*file_dir;
	ssize_t			i;

	i = 0;
	while (1)
	{
		file_dir = readdir(dir);
		if (!file_dir)
			break ;
		if (file_dir && file_dir->d_name[0] == '.')
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
	dir = opendir(path);
	if (!dir)
		return (NULL);
	i = count_dir_files(dir);
	arr = malloc(sizeof(char *) * (i + 1));
	if (!arr)
		return (closedir(dir), NULL);
	closedir(dir);
	dir = opendir(path);
	if (!dir)
		return (NULL);
	copy_names_to_arr(dir, arr);
	closedir(dir);
	quick_sort(&arr, 0, i, CASE_INSENSITVE);
	i = -1;
	while (arr[++i])
		ft_lstadd_back(&head, ft_lstnew(arr[i]));
	return (free(arr), head);
}
