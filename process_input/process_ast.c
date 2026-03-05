/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 14:24:00 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:43:41 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

t_ast	*create_node(char *token, t_token_type type)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast), Tree);
	if (!node)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->word = NULL;
	node->token = token;
	node->type = type;
	node->c_paren = false;
	node->paren = false;
	node->fd_in = -1;
	node->fd_out = -1;
	node->fds[PIPE_WRITE] = -1;
	node->fds[PIPE_READ] = -1;
	return (node);
}

t_ast	*ast_add_nodes(t_ast *left, t_ast *parent, t_ast *right)
{
	if (!parent)
		return (NULL);
	if ((left && left->paren) || (right && right->paren))
		parent->c_paren = true;
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

t_word	*node_create(char *token, t_token_type type)
{
	t_word	*node;

	if (!token || !*token)
		return (NULL);
	node = (t_word *)ft_malloc(sizeof(t_word), Tree);
	if (!node)
		return (NULL);
	node->type = type;
	node->token = token;
	node->fd = -1;
	node->next = NULL;
	return (node);
}

void	list_add(t_wordlist **wordlist, char *token)
{
	t_wordlist	*word;
	t_word		*node;

	word = *wordlist;
	node = node_create(token, Undefined);
	if (!node)
		return ;
	if (!word)
	{
		word = (t_wordlist *)ft_malloc(sizeof(t_wordlist), Tree);
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
