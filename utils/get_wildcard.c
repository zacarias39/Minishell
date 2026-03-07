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

#define BEFORE 0
#define AFTER 1

bool	is_valid(char *match[2], char *name)
{
	ssize_t	index;

	if (name[0] == '.')
		return (false);
	if (match[BEFORE])
	{
		if (ft_strncmp(match[BEFORE], name, ft_strlen(match[BEFORE])))
			return (false);
	}
	if (match[AFTER])
	{
		index = ft_strlen(name) - ft_strlen(match[AFTER]);
		if (index < 0 || ft_strncmp(match[AFTER], name + index, ft_strlen(match[AFTER])))
			return (false);
	}
	return (true);
}

/*
 * Fluxograma
 * opendir -> count_dir files -> allocate the array, dup and store it, sort,
	then return
 * add dir on list -> add on the list of
 *
 */

bool	get_before_after_wildcard(char **before, char **after, char *path)
{
	ssize_t	i;

	i = 0;
	*before = NULL;
	*after = NULL;
	while (path[i] && path[i] != '*')
		i++;
	if (!path[i])
		return (true);
	*before = path;
	if (path[i + 1])
		*after = path + (i + 1);
	path[i] = '\0';
	return (true);
}

ssize_t	count_dir_files(DIR *dir, char *match[2])
{
	struct dirent	*file_dir;
	ssize_t			i;

	i = 0;
	while (1)
	{
		file_dir = readdir(dir);
		if (!file_dir)
			break ;
		if (file_dir && !is_valid(match, file_dir->d_name))
			continue ;
		i++;
	}
	return (i);
}

void	copy_names_to_arr(DIR *dir, char **matrix, char *match[2])
{
	struct dirent	*file_dir;
	ssize_t			i;

	i = 0;
	while (1)
	{
		file_dir = readdir(dir);
		if (!file_dir)
			break ;
		if (file_dir && !is_valid(match, file_dir->d_name))
			continue ;
		matrix[i++] = ft_strdup(file_dir->d_name);
	}
	matrix[i] = NULL;
}

t_list	*get_dir_datas(char *path)
{
	char	**arr;
	char	*match[2];
	t_list	*head;
	ssize_t	i;
	DIR		*dir;

	i = 0;
	head = NULL;
	get_before_after_wildcard(&match[BEFORE], &match[AFTER], path);
	// AO Receber a string path
	// TIPOS DE WILDCARD <START *><STRING>, <STRING><END *>,
	//<START *><STRING><END *>
	// SE O MATCH FALHAR, RETORNAR A STRING ORIGINAL(path)
	//
	// Pegar o que vem antes, do wildcard
	// Pegar o que vem depois do wildcard
	//
	//
	dir = opendir(".");
	if (!dir)
		return (NULL);
	i = count_dir_files(dir, match);
	arr = malloc(sizeof(char *) * (i + 1));
	if (!arr)
		return (closedir(dir), NULL);
	closedir(dir);
	dir = opendir(path);
	if (!dir)
		return (NULL);
	copy_names_to_arr(dir, arr, match);
	closedir(dir);
	quick_sort(&arr, 0, i, CASE_INSENSITVE);
	i = -1;
	while (arr[++i])
		ft_lstadd_back(&head, ft_lstnew(arr[i]));
	return (free(arr), head);
}
