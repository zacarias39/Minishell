/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 14:02:48 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/02 23:48:54 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char **envs, const char *name)
{
	static char **tmp_env;
	size_t		begin;
	size_t		i;

	i = 0;
	begin = 0;
	if (envs)
		tmp_env = envs;
	while (tmp_env[i])
	{
		if (!strncmp(tmp_env[i], name, ft_strlen(name)))
		{
			// skips all the characters before '=' including itself and return the rest;
			while (tmp_env[i][begin++] != '=' && tmp_env[i][begin])
				;
			return (&tmp_env[i][begin]);
		}
		i++;
	}
	return (NULL);
}

