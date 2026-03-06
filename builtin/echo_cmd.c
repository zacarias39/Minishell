/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 17:14:42 by dadmendo          #+#    #+#             */
/*   Updated: 2026/02/18 19:37:58 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static inline bool	get_option(char *arg)
{
	bool	has_nl;

	has_nl = false;
	if (*arg == '-')
		arg++;
	while (*arg == 'n')
	{
		arg++;
		has_nl = true;
	}
	if (*arg)
		has_nl = false;
	return (has_nl);
}

void	echo_cmd(char **args, t_ast *word)
{
	bool	print_nl;
	size_t	i;

	i = 0;
	print_nl = true;
	while (args && args[i])
	{
		if (get_option(args[i]))
			print_nl = false;
		else
			break ;
		i++;
	}
	while (args && args[i])
	{
		ft_putstr_fd(args[i++], word->fd_out);
		if (args[i])
			ft_putstr_fd(" ", word->fd_out);
	}
	if (print_nl)
		write(word->fd_out, "\n", sizeof(char));
	last_cmd_status(EXIT_SUCCESS, true);
}
