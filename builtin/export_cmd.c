/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:23:56 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/15 11:57:02 by zcasimir         ###   ########.fr       */
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
		printf("declare -x %s\n", envars->matrix[i++]);
	return (0);
}
