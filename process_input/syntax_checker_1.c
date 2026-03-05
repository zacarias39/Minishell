/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 13:03:01 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:43:42 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

//<EXPRESSION>       ::= <CONDITION> ,'EOI' ;
t_ast	*parse_expression(void)
{
	t_ast	*root;

	root = condition();
	if (on_error(NULL, CHECK) || !expect(NULL, true))
	{
		printf("NULL\n");
		on_error(NULL, RESET);
		return (NULL);
	}
	return (root);
}

//<CONDITION>        ::= <PIPELINE> { ('||' | '&&') <PIPELINE> } ;
t_ast	*condition(void)
{
	t_ast	*parent;
	t_ast	*right;

	parent = pipeline(NULL);
	while (parent)
	{
		if (expect("||", false))
		{
			right = pipeline("cmdor> ");
			if (right == NULL)
				return (NULL);
			parent = get_parent("||", parent, right, OrCondition);
		}
		else if (expect("&&", false))
		{
			right = pipeline("cmdand> ");
			if (right == NULL)
				return (NULL);
			parent = get_parent("&&", parent, right, AndCondition);
		}
		else
			return (parent);
	}
	return (NULL);
}

//<PIPELINE>         ::= <COMMAND> { '|' <COMMAND> } ;
t_ast	*pipeline(char *prompt)
{
	t_ast	*parent;
	t_ast	*right;

	if ((prompt && get_expr(prompt)) || on_error(NULL, CHECK))
		return (NULL);
	parent = command();
	while (parent && !on_error(NULL, CHECK))
	{
		if (expect("|", false))
		{
			if (get_expr("> ") || on_error(NULL, CHECK))
				return (NULL);
			right = command();
			if (right == NULL || on_error(NULL, CHECK))
				return (NULL);
			parent = get_parent("|", parent, right, Pipeline);
		}
		else
			return (parent);
	}
	return (NULL);
}

//<COMMAND>          ::=  <COMMAND-ELEMENT> { <COMMAND-ELEMENT> } ;
t_ast	*command(void)
{
	t_ast	*parent;
	t_ast	*right;

	parent = command_element(true, false);
	while (parent && !on_error(NULL, CHECK))
	{
		if (parent->type == Command && parent->paren == false)
			right = command_element(false, true);
		else
			right = command_element(false, false);
		if (right == NULL || on_error(NULL, CHECK))
			return (parent);
		if ((parent->c_paren && right->type == Word) || right->paren)
		{
			if (right->paren && parent->type != Word)
				return (on_error("(", true), NULL);
			return (on_error(right->token, true), NULL);
		}
		if (get_elements(parent, right))
			continue ;
		parent = get_parent(NULL, parent, right, Command);
	}
	return (parent);
}
/*
<COMMAND-ELEMENT>  ::= <WORD-LIST> | <REDIRECTION-LIST> |
		'(' <CONDITION> ')' ;
*/

t_ast	*command_element(bool rigor, bool is_arg)
{
	t_ast	*parent;

	if (on_error(NULL, CHECK))
		return (NULL);
	parent = word_list(is_arg);
	if (parent)
		return (parent);
	parent = redirection_list();
	if (parent)
		return (parent);
	if (on_error(NULL, CHECK) || !expect("(", rigor))
		return (NULL);
	parent = condition();
	if (expect(")", true) && parent)
		return (parent->c_paren = parent->paren = true, parent);
	return (NULL);
}
