/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 13:03:01 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/10 13:03:05 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//<EXPRESSION>       ::= <CONDITION> ,'EOI' ;
t_ast	*parse_expression(bool is_check)
{
	t_ast	*root;

	root = condition(is_check);
	expect(NULL, true);
	if (is_check)
		ft_strtok(NULL, false, true);
	return (root);
}

//<CONDITION>        ::= <PIPELINE> { ('||' | '&&') <PIPELINE> } ;
t_ast	*condition(bool is_check)
{
	t_ast	*parent;
	t_ast	*right;

	parent = pipeline(is_check);
	while (true)
	{
		if (expect("||", false))
		{
			right = pipeline(is_check);
			if (is_check == false)
				parent = get_parent("||", parent, right, OrCondition);
		}
		else if (expect("&&", false))
		{
			right = pipeline(is_check);
			if (is_check == false)
				parent = get_parent("&&", parent, right, AndCondition);
		}
		else
			return (parent);
	}
}

//<PIPELINE>         ::= <COMMAND> { '|' <COMMAND> } ;
t_ast	*pipeline(bool is_check)
{
	t_ast	*parent;
	t_ast	*right;

	parent = command(is_check);
	while (true)
	{
		if (expect("|", false))
		{
			right = command(is_check);
			if (is_check == false)
				parent = get_parent("|", parent, right, Pipeline);
		}
		else
			return (parent);
	}
}

//<COMMAND>          ::=  <COMMAND-ELEMENT> { <COMMAND-ELEMENT> } ;
t_ast	*command(bool is_check)
{
	t_ast	*parent;
	t_ast	*right;
	int		is_true;

	is_true = false;
	parent = command_element(is_check, true, &is_true);
	while (true)
	{
		right = command_element(is_check, false, &is_true);
		if (is_true == false)
			return (parent);
		if (is_check == false)
			parent = get_parent(NULL, parent, right, Command);
	}
}

//<COMMAND-ELEMENT>  ::= <WORD-LIST> | <REDIRECTION-LIST> ;
t_ast	*command_element(bool is_check, bool rigor, int *is_true)
{
	t_ast	*parent;

	parent = word_list(is_check, is_true);
	if (*is_true == true)
		return (parent);
	return (redirection_list(is_check, rigor, is_true));
}
