/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 15:20:05 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:49 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "libft.h"
# include "signals.h"

# define MAX_HEREDOCS 17

# define MAX_SHLVL 998
# define EXPORT_ERROR 1

# define CASE_SENSITVE 1
# define CASE_INSENSITVE 2

# define NO_STATUS INT_MAX
# define GET_STATUS false
# define UPDATE_STATUS true

# define CLOSE -2

# define INVALID -1

# define RESET_DATA 0
# define UPDATE_DATA 1
# define GET_LAST_PROC 2
# define GET_N_FORKS 3

// ft_envs.c
int	                set_default_std_fd(int fd, int backup);
t_envars	*get_envs(t_envars *new);
char		*get_current_dir(char *dir, bool update);

char		**get_cmd_args(t_wordlist **list, char *cmd_name);
//
void		ft_perror(char *at, char *cmd, char *msg);
char		*ft_getenv(const char *name);
bool		ft_init_envars(char **envp, t_envars *new_envars);
t_envars	*ft_realloc(void);
void		ft_free_envars(t_envars *env_vars);

void		ft_free_matrix(char ***matrix);

char		*get_user_input(void);
void		quick_sort(char ***arr, ssize_t low, ssize_t high, int type);

char		*ft_cmd_error(char *cmd, char *cmd_name, bool is_absolute);

char		*get_cmd_path(char *cmd);
char		**cmd_paths(bool update);

void		update_env_vars(char **args);

bool		create_fork(void);

int			last_cmd_status(int status, bool update);
void		fork_calls_info(uint8_t type, pid_t *id, ssize_t *return_data);
char		*no_interactive_mode(void);

size_t		strlen_chr(char *str, char limit);

// Get DIR datas
t_list		*get_dir_datas(char *path);

char		*get_status(void);
void		get_envars_organized(void);

char		*ft_strdup2(const char *s);
char		*ft_strjoin2(char const *s1, char const *s2);

int			ft_ifs(int c);
size_t		strlen_chr(char *str, char limit);

bool		valid_identifier(char *arg);
void		ft_update_shlvl(void);
void		ft_update_pwd(void);

void		ft_free(void);
void	        collect_heredoc_fds(int fd, char op);

void		close_fds(t_ast *redir_node, t_ast *word_node);

#endif
