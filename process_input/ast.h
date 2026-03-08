/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 13:22:00 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/08 22:55:33 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "signals.h"
# include "utils.h"

# define RAISE 'P'
# define RESET 'R'
# define CHECK 'V'
# define CLEAN true
# define NO false
# define PREVIOUS true
# define NEXT false
# define QUOTES "\'\""
# define OPERATORS "&& || << >> < > | ( )"

extern int	g_sig_re;

typedef struct s_quotes
{
	char	chr;
	int		on;
}			t_quotes;

// process_input/lexical.c
char		on_error(char *token, char rigor);
char		*ft_strtok(char *str, char option, char clean);

// process_input/syntax_checker_1.c
t_ast		*parse_expression(void);
t_ast		*condition(void);
t_ast		*pipeline(char *prompt);
t_ast		*command(void);
t_ast		*command_element(bool rigor, bool is_arg);

// process_input/syntax_checker_2.c
t_ast		*redirection_list(void);
t_word		*redirection(void);
t_ast		*word_list(bool is_arg);
void		*word(bool rigor, int type);

// process_input/syntax_checker_utils.c
void		*get_list(void);
char		*get_args(t_list *l, int flag);
char		expect_word(char **token, char rigor);
bool		expect(char *expected, bool rigor);
bool		get_elements(t_ast *parent, t_ast *node);

// process_input/process_ast.c
t_ast		*create_node(char *token, t_token_type type);
t_ast		*get_parent(char *token, t_ast *left, t_ast *right,
				t_token_type type);
t_word		*node_create(char *token, t_token_type type);
void		list_add(t_wordlist **wordlist, char *token);
// process_input/expansion.c
char		*get_expansion(char *token, int type);
// process_input/expansion_1.c
int			find_wildcard(char **last);
int			quotes_del(char *token);
int			split_join(t_list **list, char *token);
int			add_token(t_list **list, char *token, char *word);
void		add_list(t_list **list, char *word);
// process_input/heredoc.c
t_word		*get_heredoc(t_word *node, size_t token_len);
int			heredoc_count(int *fd, char op);
int			get_expr(char *prompt);

#endif
