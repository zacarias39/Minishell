/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 13:04:17 by dadmendo          #+#    #+#             */
/*   Updated: 2026/02/27 15:35:00 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
*
* This function try to get the path, from the PWD var, if it works, it returns it
* But if it fails, it get from getcwd functions
*
**/

void	pwd_cmd(char **args, int fd)
{
	(void)args;
	ft_putendl_fd(get_current_dir(NULL, false), fd);
	last_cmd_status(EXIT_SUCCESS, true);
}
