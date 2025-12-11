/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 13:47:23 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/10 13:16:55 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_free_matrix(char ***matrix)
{
	size_t	i;

	i = 0;
	while ((*matrix) && (*matrix)[i])
		free((*matrix)[i++]);
	free((*matrix));
	(*matrix) = NULL;
}
