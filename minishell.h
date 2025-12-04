/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadivaldo <dadivaldo@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:07:38 by zcasimir          #+#    #+#             */
/*   Updated: 2025/12/04 11:04:35 by dadivaldo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <fcntl.h>
# include <libft.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termcap.h>
# include <termios.h>
# include <unistd.h>
# include <limits.h>

# define GREATER '>'
# define LESS '<'
# define SQUOTE '\''
# define DQUOTE '\"'
# define WILDCARD '*'
# define AND '&'
# define OR '|'
# define EOS '\0'
# define OPERATORS "<>&|"

# define HEAD 0
# define TAIL 1
# define EOS '\0'
# define BCMD "cd echo pwd export env unset exit"
# define METACHAR "<< < > >> | ="



/*
	BY Dadmendo
*/
typedef struct s_envp
{
	char **matrix;
	size_t	top;
	size_t	capacity;
}	t_envp;


typedef enum e_token_type
{
	Word,
	Append,
	Heredoc,
	Command,
	WordList,
	Pipeline,
	Wildcard,
	RedirList,
	RedirLeft,
	RedirRight,
	OrCondition,
	AndCondition,
}					t_token_type;

typedef struct s_word
{
	char			*token;
	struct s_word	*next;
}					t_word;

typedef struct s_wordlist
{
	size_t			list_len;
	t_word			*list[2];
}					t_wordlist;

typedef struct s_ast
{
	t_token_type	type;
	t_wordlist		*word;
	char			*token;
	char			**args_token;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

// utils/lib_utils.c
// bool					ft_isspace(int c);

// readline_utils.c
char				*d_get_input(void);

// process_input/lexical.c
char				print_error(char *token, char rigor);
char				*ft_strtok(char *str, char op, char clean);

// tester_functions.o

// process_input/syntax_checker.c
bool				expect(char *expected, bool rigor);
t_ast				*parse_expression(bool is_check);
t_ast				*condition(bool is_check);
t_ast				*pipeline(bool is_check);
t_ast				*command(bool is_check);
// process_input/syntax_checker1.c
t_ast				*command_element(bool is_check, bool rigor, int *is_true);
t_ast				*redirection_list(bool is_check, bool rigor, int *is_true);
t_ast				*redirection(bool is_check, bool rigor, int *is_true);
t_ast				*word_list(bool is_check, int *is_true);
void				*word(bool is_check, bool rigor, bool is_list,
						int *is_true);
// process_input/process_ast.c
t_ast				*create_node(char *token, t_token_type type);
t_ast				*get_parent(char *token, t_ast *left, t_ast *right,
						t_token_type type);
t_word				*node_create(char *token);
void	list_add(t_wordlist **wordlist, char *token);

/*
	BUILTIN
*/
int echo_cmd(char **args);
int	env_cmd(char **env_vars);
int	cd_cmd(char *path);
int	pwd_cmd(void);


/*
	FUNÇÃO DE TESTE
*/
char	**ft_get_args(t_word *tokens, size_t size);

t_envp	ft_initialize(char **envs);

void	ft_free_t_envp(t_envp *env_metadata);
char	*ft_getenv(t_envp env_metadata, const char *name);


#endif
