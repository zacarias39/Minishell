/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 14:02:48 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/02 14:06:08 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char **envs, const char *name)
{
	static char **tmp_env;
	size_t		i;

	i = 0;
	if (envs)
		tmp_env = envs;
	while (tmp_env[i])
	{
		if (!strncmp(tmp_env[i], name, ft_strlen(name)))
			return (tmp_env[i]);
		i++;
	}
	return (NULL);
}

