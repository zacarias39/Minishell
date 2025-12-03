/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 17:14:42 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/02 14:37:46 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	get_option(char **args, size_t	*i)
{
	bool print_nl;
	size_t j;
	char *aux;

	print_nl = true;
	while (args[*i])
	{
		aux = args[*i];
		j = 0;
		if (aux[j] != '-')
			break;
		while (aux[++j] == 'n')
			;
		if (aux[j])
			break;
		print_nl = false;
		*i = *i + 1;
	}
	return (print_nl);
}

int echo_cmd(char **args)
{
	bool 	print_nl;
	size_t	i;

	i = 0;
	print_nl = get_option(args, &i);
	while (args[i])
	{
		ft_putstr_fd(args[i++], STDOUT_FILENO);
		if (args[i])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (print_nl)
		write(STDOUT_FILENO, "\n", sizeof(char));
	return (0);
}
