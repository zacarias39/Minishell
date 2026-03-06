/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 13:29:01 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:14:20 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

bool	are_equal(char *env_name, char *target)
{
	if (!target && !env_name)
		return (true);
	else if (!target || !env_name)
		return (false);
	while (*target && *target == *env_name)
	{
		target++;
		env_name++;
	}
	if (!*target)
		return (true);
	return (false);
}

static void	update_t(t_envars *envars, char *env_to_unset)
{
	ssize_t	i;

	i = -1;
	if (!env_to_unset)
		return ;
	if (*env_to_unset == '_' && *(env_to_unset + 1) == '\0')
		return ;
	while (++i < envars->capacity)
	{
		if (are_equal(envars->matrix[i], env_to_unset))
		{
			free(envars->matrix[i]);
			envars->matrix[i] = NULL;
			envars->n_items--;
			if (!ft_strcmp(env_to_unset, "PATH"))
				cmd_paths(true);
			break ;
		}
	}
}

void	unset_cmd(char **args, t_ast *word)
{
	ssize_t	i;

	i = 0;
	(void)word;
	while (args && args[i])
		update_t(get_envs(NULL), args[i++]);
	last_cmd_status(EXIT_SUCCESS, true);
}
