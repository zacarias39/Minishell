/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:23:56 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/12 12:14:34 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	export_cmd(t_envars *envars, char **args)
{
	long	i;

	// Update envars if args is not NULL
	// I'm soring inside the update_envars function;
	// With this we are preventing it from sorting envar everytime
	//  we call the function even though it may already sorted;
	// Only sort if args is not NULL, new envars may be unsorted;
	if (args)
		update_envars(envars, args);
	i = 0;
	// only print if the args is NULL;
	while (i < envars->n_items && !args)
		ft_putendl_fd(envars->matrix[i++], STDIN_FILENO);
	return (0);
}
