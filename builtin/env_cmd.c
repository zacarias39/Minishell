/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:58:47 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/02 14:01:52 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_cmd(char **env_vars)
{
	size_t	i;

	i = 0;
	
	while (env_vars[i])
		ft_putendl_fd(env_vars[i++], STDIN_FILENO);
	return (1);
}
