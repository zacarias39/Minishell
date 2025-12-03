/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 13:52:35 by dadmendo          #+#    #+#             */
/*   Updated: 2025/11/27 13:52:40 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//<WORD-LIST>        ::= <WORD> { <WORD> } ;
t_ast	*word_list(bool is_check, int *is_true)
{
	t_ast	*parent;

	parent = (t_ast *)word(is_check, false, WORD, is_true);
	if (*is_true)
	{
		parent->word = (t_wordlist *)word(is_check, false, WORD_LIST, is_true);
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
	t_word	*right;

	// the parent is: RedirList;
	// inside this node will be a linked list of redirections;
	right = redirection(is_check, rigor, is_true);
	// creating the parent if right is a valid pointer, meaning it doesn't point to NULL;
	if (*is_true && is_check == false)
	{
		parent = create_node(NULL, RedirList);
		// creating the linked  list inside the parent node;
		parent->word = malloc(sizeof(t_wordlist));
		parent->word->list[HEAD] = right;
		parent->word->list[TAIL] = parent->word->list[HEAD];
	}
	while (true && *is_true)
	{
		// Making the last node point to the next found node;
		right = redirection(is_check, false, is_true);
		if (right == NULL && *is_true == false)
		{
			*is_true = true;
			return (parent);
		}
        //if the flag is_check is active, we may just conitnue to avoid malloc; 
		if (is_check == true)
			continue ;
		parent->word->list[TAIL]->next = right;
		parent->word->list[TAIL] = right;
	}
	return (parent);
}

//<REDIRECTION>      ::= '>' <WORD> | '<' <WORD> | '>>' <WORD> | '<<' <WORD> ;
t_word	*redirection(bool is_check, bool rigor, int *is_true)
{
	char	*token;

	if (expect(">", false))
	{
		token = word(is_check, true, FILENAME, is_true);
		return (node_create(token, RedirRight));
	}
	else if (expect("<", false))
	{
		token = word(is_check, true, FILENAME, is_true);
		return (node_create(token, RedirLeft));
	}
	else if (expect(">>", false))
	{
		token = word(is_check, true, FILENAME, is_true);
		return (node_create(token, Append));
	}
	else if (expect("<<", rigor))
	{
		token = word(is_check, true, FILENAME, is_true);
		return (node_create(token, Heredoc));
	}
	*is_true = false;
	return (NULL);
}

//<WORD>             ::= "? ALL PRINTABLE ASCII CHARACTERS EXCEPT FOR SPACE,
//	ACCEPTED ONLY WHEN QUOTED ?" ;
void	*word(bool is_check, bool rigor, int return_type, int *is_true)
{
	t_ast	*node;
	char	*token;

	node = NULL;
	*is_true = false;
	// if WORD_LIST flag is active it means we must return a list of words (word_list);
	if (return_type == WORD_LIST)
		return (get_list(is_check, is_true));
	token = ft_strtok(NULL, true, false);
	if (!token)
		return (print_error(token, rigor), NULL);
	if (expect_word(token, rigor) == false)
		return (NULL);
	*is_true = true;
	ft_strtok(NULL, false, false);
	if (is_check == true)
		return (node);
	// if FILENAME flag is active, it means we must just return the token string;
	if (return_type == FILENAME) 
		return (token);
	node = create_node(token, Word);
	return (node);
}