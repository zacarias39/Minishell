/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:58:01 by dadmendo          #+#    #+#             */
/*   Updated: 2026/02/16 15:49:45 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	get_fallback_dots(char *dir)
{
	size_t	i;
	int		dots;

	i = 0;
	dots = 0;
	if (dir[i++] != '/')
		return (0);
	while (dir[i])
	{
		if (dir[i] != '/' && dir[i] != '.')
			return (0);
		else if (dir[i] == '.')
			dots++;
		else
			break ;
		if (dots > 2)
			return (0);
		i++;
	}
	return (dots);
}

static inline ssize_t	ft_pop(char *str, ssize_t new_index)
{
	if (new_index <= 1)
		return (new_index);
	--new_index;
	while (new_index > 1)
	{
		if (str[new_index] == '/')
			break ;
		--new_index;
	}
	return (new_index);
}

void	ft_push(char *str, ssize_t *new_index, ssize_t *i, bool *slash)
{
	int	dots;

	dots = get_fallback_dots(str + *i);
	if (dots == 1)
		*i += 1;
	else if (dots == 2)
	{
		*new_index = ft_pop(str, *new_index);
		*i += dots;
	}
	else if (str[*i] == '/' && !*slash)
	{
		*slash = true;
		str[*new_index] = str[*i];
		*new_index += 1;
	}
	else if (str[*i] != '/')
	{
		*slash = false;
		str[*new_index] = str[*i];
		*new_index += 1;
	}
}

void	trim(char *str, ssize_t	len)
{
	ssize_t	i;
	ssize_t	new_index;
	bool	slash;

	if (len <= 0)
		return ;
	i = -1;
	new_index = 0;
	slash = false;
	while (++i < len)
		ft_push(str, &new_index, &i, &slash);
	if (new_index == 0)
		str[new_index++] = '/';
	if (new_index > 1 && str[new_index - 1] == '/')
		new_index--;
	str[new_index] = '\0';
	return ;
}
