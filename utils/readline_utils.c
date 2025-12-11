/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 15:57:56 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/08 16:30:11 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*d_get_input(void)
{
	char	*line;
	char	str[1024];
	char	*username;

	// As you can see, I'm using ft_getenv;
	username = ft_getenv(NULL, "USER");
	// if the USER's length is lower or iqual to 1020, we can give it the green color; 
	if (ft_strlen(username) <= 1020)
	{
		ft_strlcpy(str, "\033[32m", sizeof(str));
		ft_strlcat(str, username, sizeof(str));
		username = ft_strjoin(str, "$ \033[0m");
	}
	line = readline(username);
	if (*line)
		add_history(line);
	else
	{
		free(line);
		line = NULL;
	}
	free(username);
	return (line);
}
