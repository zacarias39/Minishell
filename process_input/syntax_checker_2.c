/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 13:52:35 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:43:43 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

//<REDIRECTION-LIST> ::= <REDIRECTION> { <REDIRECTION> } ;
t_ast	*redirection_list(void)
{
	t_ast	*parent;
	t_word	*right;

	if (on_error(NULL, CHECK))
		return (NULL);
	right = redirection();
	if (right)
	{
		parent = create_node(NULL, RedirList);
		parent->word = ft_malloc(sizeof(t_wordlist), Tree);
		parent->word->list[HEAD] = right;
		parent->word->list[TAIL] = right;
		while (true)
		{
			right = redirection();
			if (right == NULL)
				return (parent);
			parent->word->list[TAIL]->next = right;
			parent->word->list[TAIL] = right;
		}
	}
	return (NULL);
}

//<REDIRECTION>      ::= '>' <WORD> | '<' <WORD> | '>>' <WORD> | '<<' <WORD> ;
t_word	*redirection(void)
{
	char			*token;
	t_token_type	type;

	if (expect(">", false))
		type = RedirRight;
	else if (expect("<", false))
		type = RedirLeft;
	else if (expect(">>", false))
		type = Append;
	else if (expect("<<", false))
		type = Heredoc;
	else
		return (NULL);
	if (type == Heredoc)
		token = word(true, Heredoc);
	else
		token = word(true, RedirList);
	if (!token)
		return (NULL);
	if (type == Heredoc)
		return (get_heredoc(node_create(token, Heredoc), ft_strlen(token)));
	return (node_create(token, type));
}

//<WORD-LIST>        ::= <WORD> { <WORD> } ;
t_ast	*word_list(bool is_arg)
{
	t_ast	*parent;
	char	*token;

	parent = NULL;
	token = ft_strtok(NULL, PREVIOUS, NO);
	if (is_arg == false)
		parent = (t_ast *)word(false, Word);
	else if (expect_word(&token, false))
	{
		parent = (t_ast *)ft_malloc(sizeof(t_ast), Tree);
		parent->type = Word;
	}
	if (parent != NULL)
	{
		parent->word = (t_wordlist *)word(false, WordList);
		if (parent->word && is_arg == true)
			parent->token = parent->word->list[HEAD]->token;
		return (parent);
	}
	return (NULL);
}

//<WORD>             ::= "? ALL PRINTABLE ASCII CHARACTERS EXCEPT FOR SPACE,
//	ACCEPTED ONLY WHEN QUOTED ?" ;
void	*word(bool rigor, int type)
{
	char	*token;

	if (type == WordList)
		return (get_list());
	token = ft_strtok(NULL, PREVIOUS, NO);
	if (!token)
		return (on_error(token, rigor), NULL);
	if (expect_word(&token, rigor) == false)
		return (NULL);
	ft_strtok(NULL, NEXT, NO);
	if (type != Word)
		return (token);
	return (create_node(token, Word));
}
