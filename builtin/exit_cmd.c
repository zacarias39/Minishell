/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 12:05:23 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:18:47 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	ignore_space(const char *nptr, ssize_t *i)
{
	ssize_t	aux;

	aux = *i;
	while ((nptr[aux] >= 9 && nptr[aux] <= 13) || nptr[aux] == 32)
		aux++;
	*i = aux;
}

int	get_n_sign(const char *nptr, ssize_t *i)
{
	int	sign;

	sign = 1;
	if (!nptr)
		return (2);
	ignore_space(nptr, i);
	if (!nptr[*i])
		return (2);
	if ((nptr[*i] == '+') || (nptr[*i] == '-'))
	{
		if (nptr[*i] == '-')
			sign = -1;
		*i += 1;
	}
	if (!ft_isdigit(nptr[*i]))
		return (2);
	return (sign);
}

/// There is a bug, try to run with ft_atol("-");
// I think the var has_num solve the problem

bool	ft_atol(const char *nptr, long long *ans)
{
	long long	result;
	int			sign;
	ssize_t		i;

	i = 0;
	*ans = 2;
	result = 0;
	sign = get_n_sign(nptr, &i);
	if (sign == 2)
		return (false);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (sign == 1 && result > (LLONG_MAX - (nptr[i] - '0')) / 10)
			return (false);
		else if (-result < (LLONG_MIN + (nptr[i] - '0')) / 10)
			return (false);
		result = (result * 10) + (nptr[i] - '0');
		i++;
	}
	ignore_space(nptr, &i);
	if (nptr[i])
		return (false);
	*ans = result * sign;
	return (true);
}

void	exit_cmd(char **args, t_ast *word)
{
	long long	exit_value;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (args && *args)
	{
		if (!ft_atol(*args, &exit_value))
		{
			ft_perror(MSH, "exit", NULL);
			ft_perror(NULL, args[0], "numeric argument required");
		}
		else if (args[1])
		{
			ft_perror(MSH, "exit", TOO_MANY);
			last_cmd_status(1, UPDATE_DATA);
			return ;
		}
	}
	else
		exit_value = last_cmd_status(NO_STATUS, GET_STATUS);
	close_fds(NULL, word);
	ft_free();
	exit(last_cmd_status(exit_value, UPDATE_STATUS));
}
