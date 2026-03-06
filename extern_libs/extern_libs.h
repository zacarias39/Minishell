/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extern_libs.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 15:10:01 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 19:29:04 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXTERN_LIBS_H
# define EXTERN_LIBS_H

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termcap.h>
# include <termios.h>
# include <unistd.h>

# define PIPE_READ 0
# define PIPE_WRITE 1

# define GREATER '>'
# define LESS '<'
# define SQUOTE '\''
# define DQUOTE '\"'
# define WILDCARD '*'
# define AND '&'
# define OR '|'
# define EOS '\0'
# define OPERATOR "<>&|()"

# define HEAD 0
# define TAIL 1
# define EOS '\0'
# define BCMD "cd echo pwd export env unset exit"
# define METACHAR "<< < > >> | ="

# define MSH "mnsh"
# define DEFAULT_PROMPT "\001\033[32m\002mnsh$ \001\033[0m\002"

# define TOO_MANY "too many arguments"

# define WORD_LIST 1
# define FILENAME 2
# define WORD 3

# define MATRIX_SIZE 1

# define SINTAXE_ERROR 2

typedef struct s_lst_fds
{
	int					fds[2];
	struct s_lst_fds	*next;
}	t_lst_fds;

typedef enum e_token_type
{
	Word,
	Paren,
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

typedef struct s_word
{
	t_token_type	type;
	int				fd;
	char			*token;
	struct s_word	*next;
}					t_word;

typedef struct s_wordlist
{
	size_t			list_len;
	t_word			*list[2];
}					t_wordlist;

typedef enum e_data_name
{
	Envs,
	ReturnValue,
}					t_data_name;

typedef struct s_envars
{
	char			**matrix;
	ssize_t			capacity;
	ssize_t			n_items;
}					t_envars;

typedef struct s_ast
{
	t_token_type	type;
	t_wordlist		*word;
	t_lst_fds		*fds_lst;
	int				fds[2];
	int				fd_in;
	int				fd_out;
	int				c_paren;
	int				fork_redir;
	int				paren;
	char			*token;
	char			**args_token;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

typedef struct s_metadata
{
	t_ast			*root;
	t_envars		envs;
}					t_metadata;

typedef enum e_commom_error
{
	Notfound = 127,
	Denied = 126
}					t_common_error;

typedef enum e_malloc
{
	Free,
	Tree,
	TreeFree,
	General,
}					t_malloc;

void				*ft_malloc(size_t size, t_malloc op);

#endif
