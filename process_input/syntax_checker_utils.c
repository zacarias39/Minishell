/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:40:01 by zcasimir          #+#    #+#             */
/*   Updated: 2026/03/04 16:43:44 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

// EXPECTS THE TOKEN TO BE THE SAME AS THE EXPECTED STRING
bool	expect(char *expected, bool rigor)
{
	char	*token;

	token = ft_strtok(NULL, PREVIOUS, NO);
	if (ft_strcmp(expected, token))
		return (on_error(token, rigor));
	ft_strtok(NULL, NEXT, NO);
	return (true);
}

char	expect_word(char **token, char rigor)
{
	char	quotes;
	ssize_t	i;

	i = -1;
	quotes = 0;
	if (*token == NULL)
		return (on_error(*token, rigor));
	while (token[0][++i])
	{
		if (!quotes && ft_strchr(QUOTES, token[0][i]))
			quotes = token[0][i];
		else if (quotes == token[0][i])
			quotes = 0;
		if (!quotes && ft_strchr(OPERATOR, token[0][i]))
			return (on_error(*token, rigor));
	}
	if (quotes)
		return (on_error("unclosed quotes", true));
	return (true);
}

bool	get_elements(t_ast *parent, t_ast *node)
{
	t_ast	*child;

	if (parent->type != Command)
		return (false);
	if (parent->left->type == node->type)
		child = parent->left;
	else
		child = parent->right;
	if (child->word)
		child->word->list[TAIL]->next = node->word->list[HEAD];
	else
		child->word = node->word;
	child->word->list[TAIL] = node->word->list[TAIL];
	return (true);
}

char	*get_args(t_list *l, int flag)
{
	static t_list	*list;
	static char		*token;

	if (l)
		list = l;
	if (flag == PREVIOUS)
		return (token);
	if (!list)
		return (token = NULL, list = NULL, NULL);
	l = list;
	list = list->next;
	token = l->content;
	free(l);
	return (token);
}

void	*get_list(void)
{
	t_wordlist	*list;
	char		*token;

	list = NULL;
	token = ft_strtok(NULL, PREVIOUS, NO);
	while (token != NULL)
	{
		if (expect_word(&token, false) == false)
			break ;
		list_add(&list, token);
		token = ft_strtok(NULL, NEXT, NO);
	}
	return (list);
}
