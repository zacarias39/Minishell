/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 12:55:38 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/16 16:22:59 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	arrlen(char **arr)
{
	size_t	len;
	
	len = 0;
	while (arr && arr[len])
		len++;
	return (len);
}

int	cd_cmd(char **path)
{
	char	*dir;
	int		 status;

	dir = ft_getenv(NULL, "HOME");
	if (path)
		dir = path[1];
	// counting how many strings the array has
	if (arrlen(path) >= 2)
	{
		ft_perror("mnsh", "cd", "too many arguments");
		return (1);
	}
	status = chdir(dir);
	// on error throw this message;
	if (status == -1)
	{
		ft_perror("mnsh", NULL, NULL);
		ft_perror("cd", dir, "No such file or directory");
		return (1);
	}
	return (status);
}
