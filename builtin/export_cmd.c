/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:23:56 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/11 13:34:05 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	export_cmd(char **args, size_t length)
{
	size_t	i;

	quick_sort(&args, 0, length);
	i = 0;
	while (i < length)
		ft_putendl_fd(args[i++], STDIN_FILENO);
	return (0);
}
