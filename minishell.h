/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:07:38 by zcasimir          #+#    #+#             */
/*   Updated: 2025/12/16 16:23:29 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <fcntl.h>
# include <libft.h>
# include <limits.h>
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

# define WORD_LIST 1
# define FILENAME 2
# define WORD 3

# define MATRIX_SIZE 100

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
	Undefined
}					t_token_type;

typedef	enum e_commom_error
{
	Notfound = 127,
	Denied = 128
}	t_common_error;

typedef struct s_word
{
	t_token_type	type;
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

typedef struct s_envars
{
	char			**matrix;
	ssize_t			capacity;
	ssize_t			n_items;
}					t_envars;

// Dadmendo 11/12/2025
typedef struct s_metadata
{
	t_ast		*root;
	t_envars	envs;
}	t_metadata;



// utils/lib_utils.c
// bool					ft_isspace(int c);

// readline_utils.c
char				*d_get_input(void);

// process_input/lexical.c
char				print_error(char *token, char rigor);
char				*ft_strtok(char *str, char op, char clean);

// tester_functions.o

// process_input/syntax_checker_1.c
t_ast				*parse_expression(bool is_check);
t_ast				*condition(bool is_check);
t_ast				*pipeline(bool is_check);
t_ast				*command(bool is_check);
t_ast				*command_element(bool is_check, bool rigor, int *is_true);
// process_input/syntax_checker_2.c
t_ast				*redirection_list(bool is_check, bool rigor, int *is_true);
t_word				*redirection(bool is_check, bool rigor, int *is_true);
t_ast				*word_list(bool is_check, int *is_true);
void				*word(bool is_check, bool rigor, int return_type,
						int *is_true);
// process_input/syntax_checker_utils.c
bool				expect(char *expected, bool rigor);
char				expect_word(char *token, char rigor);
void				*get_list(bool is_check, int *is_true);
// process_input/process_ast.c
t_ast				*create_node(char *token, t_token_type type);
t_ast				*get_parent(char *token, t_ast *left, t_ast *right,
						t_token_type type);
t_word				*node_create(char *token, t_token_type type);
void				list_add(t_wordlist **wordlist, char *token);

// builtin/
int					echo_cmd(char **args);
int					env_cmd(char **env_vars);
// changed from a single pointer to a double pointer
int					cd_cmd(char **path);
int					pwd_cmd(void);
	// Dadmendo 11/12/2025
//int					export_cmd(char **args, size_t length);
int					export_cmd(t_envars *envars, char **args);

// utils/ft_*
void    ft_perror(char *at, char *cmd, char *msg);
char		*ft_getenv(t_envars *envars, const char *name);
bool		ft_init_envars(char **envp, t_envars *new_envars);
t_envars	ft_realloc(t_envars *old_envars);
void		ft_free_envars(t_envars *old_envars);
// 		Dadmendo 11/12/2025 - Export to Update
void	update_envars(t_envars *envars, char **envs);
char		**matrix_from_list(t_wordlist **list);
void		ft_free_matrix(char ***matrix);
char	*ft_cmd_error(char *cmd, char *cmd_name, bool is_absolute);

char		*get_cmd_path(char *cmd);
char	**cmd_paths(bool update);


// 	Quick_sort
// 		Dadmendo 11/12/2025
void	quick_sort(char ***arr, ssize_t low, ssize_t high);

#endif
