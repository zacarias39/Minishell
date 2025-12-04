/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadivaldo <dadivaldo@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 11:19:54 by dadivaldo         #+#    #+#             */
/*   Updated: 2025/12/04 11:20:53 by dadivaldo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_env(t_envp metadata)
{
	size_t	i;

	i = 0;
	while (i < metadata.capacity)
	{
		ft_putendl_fd(metadata.matrix[i], STDOUT_FILENO);
	}
}