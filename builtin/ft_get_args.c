/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadivaldo <dadivaldo@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:11:11 by dadivaldo         #+#    #+#             */
/*   Updated: 2025/12/02 16:18:53 by dadivaldo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	**ft_get_args(t_word *tokens, size_t size)
{
	char	**args;
	size_t	i = 0;

	args = malloc(sizeof(char *) + (size + 1));
	while (tokens) {
			args[i++] = tokens->token;
			tokens = tokens->next;
	}
	args[i] = NULL;
	return (args);
}
