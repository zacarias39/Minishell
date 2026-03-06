/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 11:28:09 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:51:39 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "utils.h"

# define GETCWD_ERROR 2

# define CD_PATH_ERROR "Use a relative or a full path"

typedef struct s_builtin
{
	int		error;
	void	(*cmd_exec)(char **args, t_ast *word);
	bool	(*redir)(t_ast *, int *, int *, bool);
	char	**args;
}			t_builtin;

void		env_cmd(char **args, t_ast *word);
void		pwd_cmd(char **args, t_ast *word);
void		cd_cmd(char **path, t_ast *word);
void		unset_cmd(char **args, t_ast *word);
void		exit_cmd(char **args, t_ast *word);
void		echo_cmd(char **args, t_ast *word);
void		export_cmd(char **args, t_ast *word);
void		trim(char *s, ssize_t len);
void		fallback_dots(char **new_dir, char *dir);

#endif
