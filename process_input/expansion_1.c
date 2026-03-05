/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:17:50 by zcasimir          #+#    #+#             */
/*   Updated: 2026/03/04 16:43:36 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

int	quotes_del(char *token)
{
	static int	quotes;

	if (!token)
		return (quotes);
	if (!quotes && ft_strchr(QUOTES, *token))
	{
		quotes = *token;
		ft_memmove(token, token + 1, ft_strlen(token + 1) + 1);
	}
	else if (quotes == *token)
	{
		quotes = 0;
		ft_memmove(token, token + 1, ft_strlen(token + 1) + 1);
	}
	else
		return (false);
	return (true);
}

void	add_list(t_list **list, char *word)
{
	t_list	*last;

	if (!word || !*word)
		return ;
	last = ft_lstlast(*list);
	if (last)
		last->content = ft_strjoin(last->content, word);
	else
		ft_lstadd_back(list, ft_lstnew(word));
}

int	add_token(t_list **list, char *token, char *word)
{
	static int	words;
	t_list		*last;

	last = NULL;
	if (ft_isspace(*token) || !*(token + 1))
	{
		if (words == 0 && ++words)
			last = ft_lstlast(*list);
		if (*(token + 1))
			*token = 0;
		else
			words = 0;
		if (last && !ft_isspace(*word))
			last->content = ft_strjoin(last->content, word);
		else
			ft_lstadd_back(list, ft_lstnew(word));
	}
	else
		return (false);
	return (true);
}

int	split_join(t_list **list, char *token)
{
	char	*word;

	if (!token || !*token)
		return (0);
	word = token;
	while (*token)
	{
		if (add_token(list, token, word))
		{
			word = token + 1;
			while (ft_isspace(*(++token)) && word++)
				;
			continue ;
		}
		token++;
	}
	add_list(list, word);
	return (0);
}
