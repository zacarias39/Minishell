/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:30:42 by zcasimir          #+#    #+#             */
/*   Updated: 2026/03/04 16:43:34 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

int	add_var(t_list **list, char **word, char **s, int quotes)
{
	static char	str[10256];
	static int	len;
	char		*token;
	char		*expanded;

	token = *s + 1;
	while (ft_isalnum(*token))
		str[len++] = *token++;
	*word = token;
	*s = token - 1;
	str[len] = 0;
	len = 0;
	expanded = ft_getenv(str);
	if (!expanded || !*expanded)
		return (0);
	if (quotes == 0)
		return (split_join(list, ft_strdup(expanded)));
	add_list(list, expanded);
	return (0);
}

int	check_wildcard(t_list **list, char *start, char *token, int type)
{
	t_list	*last;

	if ((token > start && *(token - 1)) || quotes_del(NULL) || type == Heredoc)
		return (false);
	while (*token == '*')
		token++;
	if (*token)
		return (false);
	last = ft_lstlast(*list);
	if (last)
		last->next = get_dir_datas(get_current_dir(NULL, false));
	else
		*list = get_dir_datas(get_current_dir(NULL, false));
	return (true);
}

void	check_env(t_list **list, char **s, char **word, int type)
{
	char	*token;
	char	quotes;
	char	next;

	token = *s;
	next = *(token + 1);
	quotes = quotes_del(NULL);
	if (type != Heredoc && !quotes && ft_strchr(QUOTES, next))
		quotes = true;
	if (ft_isalnum(next) || quotes == true || next == '?')
	{
		*token = 0;
		add_list(list, *word);
		if (next == '?')
		{
			token++;
			*word = ++token;
			add_list(list, get_status());
		}
		else if (type == Heredoc)
			add_var(list, word, &token, true);
		else
			add_var(list, word, &token, quotes_del(NULL));
		*s = token;
	}
}

void	get_var(t_list **list, char *token, int type)
{
	char	*word;
	char	*start;

	start = token;
	word = token;
	while (token && *token)
	{
		if (type != Heredoc && quotes_del(token))
			continue ;
		if (*token == '$' || *token == '*')
		{
			if (quotes_del(NULL) != '\'')
			{
				if (!*list && check_wildcard(list, start, token, type))
					return ;
				if (*token == '$')
					check_env(list, &token, &word, type);
			}
		}
		token++;
	}
	add_list(list, word);
}

char	*get_expansion(char *token, int type)
{
	char	*token_tmp;
	t_list	*list;
	ssize_t	i;

	i = -1;
	list = NULL;
	while (token && token[++i] && token[i] != '$')
		;
	if (type == RedirList)
		token_tmp = ft_strdup(token);
	get_var(&list, token, type);
	if (type == RedirList && ft_lstsize(list) != 1)
	{
		ft_perror("mnsh", token_tmp, "ambigous redirect");
		return (NULL);
	}
	return (get_args(list, NEXT));
}
