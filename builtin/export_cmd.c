/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:23:56 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 13:57:33 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	print_envs(char *s, int fd)
{
	bool	equal_sign;
	size_t	i;

	if (!s || *s == '\0')
		return ;
	equal_sign = false;
	i = 0;
	ft_putstr_fd("declare -x ", fd);
	while (s[i])
	{
		write(fd, &s[i], sizeof(char));
		if (s[i] == '=' && equal_sign == false)
		{
			ft_putchar_fd('\"', fd);
			equal_sign = true;
		}
		i++;
	}
	if (equal_sign == true)
		ft_putstr_fd("\"", fd);
	ft_putstr_fd("\n", fd);
}

char	**create_strs_copy_and_sort(void)
{
	const t_envars	*envars = get_envs(NULL);
	ssize_t			i;
	ssize_t			j;
	char			**matrix;

	matrix = malloc(sizeof(char *) * (envars->n_items + 1));
	if (!matrix)
		return (ft_perror(MSH, "malloc", strerror(errno)), NULL);
	j = -1;
	i = 0;
	while (++j < envars->capacity)
	{
		if (envars->matrix[j] != NULL && *(envars->matrix[j]) != '\0')
			matrix[i++] = envars->matrix[j];
	}
	matrix[i] = NULL;
	quick_sort(&matrix, 0, i, CASE_SENSITVE);
	return (matrix);
}

// Update envars if args is not NULL
// I'm soring inside the update_env_vars function;
// With this we are preventing it from sorting envar everytime
//  we call the function even though it may already sorted;
// Only sort if args is not NULL, new envars may be unsorted;

void	export_cmd(char **args, t_ast *word)
{
	ssize_t	i;
	char	**matrix;

	last_cmd_status(EXIT_SUCCESS, UPDATE_DATA);
	if (args && *args)
		return (update_env_vars(args));
	matrix = create_strs_copy_and_sort();
	if (!matrix)
		return ;
	i = -1;
	while (matrix[++i])
	{
		if (!(matrix[i][0] == '_' && matrix[i][1] == '='))
			print_envs(matrix[i], word->fd_out);
	}
	free(matrix);
}
