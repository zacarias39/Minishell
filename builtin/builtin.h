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
	int		fd_in;
	int		fd_out;
	void	(*cmd_exec)(char **args, int fd);
	bool	(*redir)(t_ast *, int *, int *);
	char	**args;
}			t_builtin;

void		env_cmd(char **args, int fd);
void		pwd_cmd(char **args, int fd);
void		cd_cmd(char **path, int fd);
void		unset_cmd(char **args, int fd);
void		exit_cmd(char **args, int fd);
void		echo_cmd(char **args, int fd);
void		export_cmd(char **args, int fd);
void		trim(char *s, ssize_t len);
void		fallback_dots(char **new_dir, char *dir);

#endif
