/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 12:55:38 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/12 12:01:44 by zcasimir         ###   ########.fr       */
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
	int	status;

	// counting how many strings the array has
	if (arrlen(path) >= 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	status = chdir(path[0]);
	// on error throw this message;
	if (status == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path[0], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (1);
	}
	return (status);
}
