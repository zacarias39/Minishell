/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadivaldo <dadivaldo@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 14:24:00 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/02 16:26:52 by dadivaldo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_node(char *token, t_token_type type)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->word = NULL;
	node->token = token;
	node->type = type;
	node->args_token = NULL;
	return (node);
}

t_ast	*ast_add_nodes(t_ast *left, t_ast *parent, t_ast *right)
{
	if (!parent)
		return (NULL);
	parent->left = left;
	parent->right = right;
	return (parent);
}

t_ast	*get_parent(char *token, t_ast *left, t_ast *right, t_token_type type)
{
	t_ast	*parent;

	if (!right)
		return (NULL);
	parent = create_node(token, type);
	if (!parent)
		return (NULL);
	ast_add_nodes(left, parent, right);
	return (parent);
}

t_word	*node_create(char *token)
{
	t_word	*node;

	if (!token || !*token)
		return (NULL);
	node = (t_word *)malloc(sizeof(t_wordlist));
	if (!node)
		return (NULL);
	node->token = token;
	node->next = NULL;
	return (node);
}

void	list_add(t_wordlist **wordlist, char *token)
{
	t_wordlist	*word;
	t_word		*node;

	word = *wordlist;
	node = node_create(token);
	if (!node)
		return ;
	if (!word)
	{
		word = (t_wordlist *)malloc(sizeof(t_wordlist));
		word->list_len = 1;
		word->list[HEAD] = node;
		word->list[TAIL] = word->list[HEAD];
		*wordlist = word;
		return ;
	}
	word->list_len += 1;
	word->list[TAIL]->next = node;
	word->list[TAIL] = node;
	*wordlist = word;
	return ;
}
