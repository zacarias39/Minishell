/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 13:47:23 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/12 00:53:43 by zcasimir         ###   ########.fr       */
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
