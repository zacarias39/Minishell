/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 13:04:17 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/02 13:52:28 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_cmd(void)
{
	char	*dir_path;

	dir_path = getcwd(NULL, USHRT_MAX);
	if (!dir_path)
		return (-1);
	ft_putendl_fd(dir_path, STDOUT_FILENO);
	free(dir_path);
	return (0);
}
