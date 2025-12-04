/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadivaldo <dadivaldo@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 13:35:45 by zcasimir          #+#    #+#             */
/*   Updated: 2025/12/02 16:07:19 by dadivaldo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 14:52:08 by dadmendo          #+#    #+#             */
/*   Updated: 2025/11/28 13:35:43 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
<EXPRESSION>       ::= <CONDITION> ,'EOI' ;
<CONDITION>        ::= <PIPELINE> { ('||' | '&&') <PIPELINE> } ;
<PIPELINE>         ::= <COMMAND> { '|' <COMMAND> } ;
<COMMAND>          ::=  <COMMAND-ELEMENT> { <COMMAND-ELEMENT> } ;
<COMMAND-ELEMENT>  ::= <WORD-LIST> | <REDIRECTION-LIST> ;
<REDIRECTION-LIST> ::= <REDIRECTION> { <REDIRECTION> } ;
<REDIRECTION>      ::= '>' <WORD> | '<' <WORD> | '>>' <WORD> | '<<' <WORD> ;
<WORD-LIST>        ::= <WORD> { <WORD> } ;
<WORD>             ::= "? ALL PRINTABLE ASCII CHARACTERS EXCEPT FOR SPACE,
	ACCEPTED ONLY WHEN QUOTED ?" ;
*/

// EXPECTS THE TOKEN TO BE THE SAME AS THE EXPECTED STRING
bool	expect(char *expected, bool rigor)
{
	char	*token;

	token = ft_strtok(NULL, true, false);
	if (ft_strcmp(expected, token))
		return (print_error(token, rigor));
	ft_strtok(NULL, false, false);
	return (true);
}

//<EXPRESSION>       ::= <CONDITION> ,'EOI' ;
t_ast	*parse_expression(bool is_check)
{
	t_ast	*root;

	root = condition(is_check);
	expect(NULL, true);
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

	is_true = 0;
	parent = command_element(is_check, true, &is_true);
	while (true)
	{
		right = command_element(is_check, false, &is_true);
		if (right == NULL && is_true == false)
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
	if (parent || *is_true == true)
		return (parent);
	return (redirection_list(is_check, rigor, is_true));
}

//<WORD-LIST>        ::= <WORD> { <WORD> } ;
t_ast	*word_list(bool is_check, int *is_true)
{
	t_ast	*parent;

	parent = (t_ast *)word(is_check, false, false, is_true);
	if (*is_true)
	{
		parent->word = (t_wordlist *)word(is_check, false, true, is_true);
		*is_true = true;
		return (parent);
	}
	*is_true = false;
	return (parent);
}

//<REDIRECTION-LIST> ::= <REDIRECTION> { <REDIRECTION> } ;
t_ast	*redirection_list(bool is_check, bool rigor, int *is_true)
{
	t_ast	*parent;
	t_ast	*right;

	parent = redirection(is_check, rigor, is_true);
	while (true && *is_true)
	{
		right = redirection(is_check, false, is_true);
		if (right == NULL && *is_true == false)
		{
			*is_true = true;
			return (parent);
		}
		if (is_check == false)
			parent = get_parent(NULL, parent, right, RedirList);
	}
	*is_true = false;
	return (parent);
}

//<REDIRECTION>      ::= '>' <WORD> | '<' <WORD> | '>>' <WORD> | '<<' <WORD> ;
t_ast	*redirection(bool is_check, bool rigor, int *is_true)
{
	t_ast	*parent;

	if (expect(">", false))
	{
		parent = word(is_check, true, false, is_true);
		return (get_parent(">", NULL, parent, RedirRight));
	}
	else if (expect("<", false))
	{
		parent = word(is_check, true, false, is_true);
		return (get_parent("<", NULL, parent, RedirLeft));
	}
	else if (expect(">>", false))
	{
		parent = word(is_check, true, false, is_true);
		return (get_parent(">>", NULL, parent, Append));
	}
	else if (expect("<<", rigor))
	{
		parent = word(is_check, true, false, is_true);
		return (get_parent("<<", NULL, parent, Heredoc));
	}
	*is_true = false;
	return (NULL);
}

char	expect_word(char *token, char rigor)
{
	static int	double_quote;
	static int	single_quote;
	static int	is_on;
	int			i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '\'' && !double_quote)
		{
			single_quote = 1 - single_quote;
			is_on = single_quote;
		}
		else if (token[i] == '\"' && !single_quote)
		{
			double_quote = 1 - double_quote;
			is_on = double_quote;
		}
		if (is_on && token[i + 1] == '\0')
			return (print_error(token, true));
		if (ft_strchr(OPERATORS, token[i]) && !is_on)
			return (is_on = 0, print_error(token, rigor));
		i++;
	}
	return (true);
}

void	*get_list(bool is_check, int *is_true)
{
	t_wordlist	*list;
	char		*token;
	int			i;

	i = 0;
	list = NULL;
	token = ft_strtok(NULL, true, false);
	while (token != NULL)
	{
		if (expect_word(token, false) == false)
			break ;
		if (is_check == false)
			list_add(&list, token);
		token = ft_strtok(NULL, false, false);
		i++;
	}
	if (i >= 1)
		*is_true = true;
	return (list);
}

//<WORD>             ::= "? ALL PRINTABLE ASCII CHARACTERS EXCEPT FOR SPACE,
//	ACCEPTED ONLY WHEN QUOTED ?" ;
void	*word(bool is_check, bool rigor, bool is_list, int *is_true)
{
	t_ast	*node;
	char	*token;

	node = NULL;
	*is_true = false;
	if (is_list == true)
		return (get_list(is_check, is_true));
	token = ft_strtok(NULL, true, false);
	if (!token)
		return (print_error(token, rigor), NULL);
	if (expect_word(token, rigor) == false)
		return (NULL);
	printf("token: %s\n", token);
	*is_true = true;
	ft_strtok(NULL, false, false);
	if (is_check == true)
		return (node);
	node = create_node(token, Word);
	return (node);
}
