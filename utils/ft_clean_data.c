/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 13:47:23 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:36 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	ft_free_matrix(char ***matrix)
{
	size_t	i;

	i = 0;
	while ((*matrix) && (*matrix)[i])
		free((*matrix)[i++]);
	free((*matrix));
	(*matrix) = NULL;
}

void	*ft_malloc(size_t size, t_malloc op)
{
	static t_list	*tree;
	static t_list	*general;
	t_list			*node;
	void			*content;

	if (op == TreeFree)
		return (ft_lstclear(&tree, &free), NULL);
	if (op == Free)
	{
		ft_lstclear(&general, &free);
		ft_lstclear(&tree, &free);
		return (NULL);
	}
	content = ft_calloc(size, sizeof(void *));
	if (!content)
		return (NULL);
	node = ft_lstnew(content);
	if (!node)
		return (free(content), NULL);
	if (op == Tree)
		return (ft_lstadd_front(&tree, node), tree->content);
	else
		ft_lstadd_front(&general, node);
	return (content);
}

void	collect_heredoc_fds(int fd, char op)
{
	static int	fds[MAX_HEREDOCS];
	static int	len;

	if (op == CLOSE)
	{
		while (len)
		{
			--len;
			if (fds[len] >= 3)
				close(fds[len]);
		}
		len = 0;
		return ;
	}
	fds[len++] = fd;
}

void	ft_free_envars(t_envars *env_vars)
{
	ssize_t	i;

	i = 0;
	while (i < env_vars->capacity)
	{
		if (env_vars->matrix[i])
			free(env_vars->matrix[i]);
		i++;
	}
	free(env_vars->matrix);
	env_vars->capacity = 0;
	env_vars->n_items = 0;
}

void	ft_free(void)
{
	ft_malloc(0, Free);
	ft_free_envars(get_envs(NULL));
	free(get_current_dir(NULL, false));
	set_default_stdin(CLOSE);
	rl_clear_history();
}
