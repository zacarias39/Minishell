/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:58:47 by dadmendo          #+#    #+#             */
/*   Updated: 2026/02/27 15:34:40 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	env_cmd(char **args, t_ast *word)
{
	ssize_t		i;
	t_envars	*envars;
	char		*aux;

	i = 0;
	(void)args;
	envars = get_envs(NULL);
	while (i < envars->capacity)
	{
		aux = envars->matrix[i];
		if ((aux && *aux) && ft_strchr(aux, '='))
			ft_putendl_fd(aux, word->fd_out);
		i++;
	}
}
