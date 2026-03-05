/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversing_ast.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:17:29 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 19:33:25 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRAVERSING_AST_H
# define TRAVERSING_AST_H

# include "ast.h"
# include "builtin.h"

# define REDIR_ERROR 1

# define IN_FD 0
# define OUT_FD 1

# define SIGINT_CODE 130
# define SIGQUIT_CODE 131
# define JMP_CMD_NAME 1

void		add_pipes_to_child(t_ast *head);
bool		check_file(char *file_name, t_token_type type);
int			process_exit_status(int status, int *sig);
void		wait_child(void);

void		traversing_ast(t_ast *head, bool from_fork);
void		traversing_command(t_ast *node, bool from_fork);
void		ft_execute_cmd(t_ast *word_node, t_ast *redir_node, bool from_fork);
void		update_cmd_fds(t_ast *word_node);

bool		builtin_cmd(t_ast *word_node, t_ast *redir_node, bool from_fork);

bool		ft_redirlist(t_ast *head, int *input, int *output);
void		close_ast_node_fds(t_ast *head);
t_builtin	get_builtin_info(t_ast *node, t_ast *redir);

bool		check_and_create_pipe(t_ast *head, bool *from_fork);
bool		or_and_condition(t_ast *head);

void		close_fds(t_ast *redir_node, t_ast *word_node);

t_lst_fds	*ft_lstnew_fd(int fds[2]);
void		ft_lstadd_front_fd(t_lst_fds **lst, t_lst_fds *new);
void		close_pipes(t_lst_fds **head);

#endif
