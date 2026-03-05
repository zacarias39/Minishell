/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 11:46:34 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:54:33 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	export_error(char *arg)
{
	ft_perror(MSH, "export", NULL);
	ft_putchar_fd('`', STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("\': not a valid identifier", STDERR_FILENO);
	last_cmd_status(EXPORT_ERROR, UPDATE_DATA);
}

int	ft_update_shlvl_aux(char **shlvl)
{
	int			aux;
	t_envars	*envars;

	envars = get_envs(NULL);
	if (envars == NULL)
		return (INVALID);
	*shlvl = ft_getenv("SHLVL");
	if (!*shlvl)
		aux = 0;
	else
		aux = ft_atoi(*shlvl);
	if (aux < 0)
		aux = 0;
	else if (aux > MAX_SHLVL)
	{
		++aux;
		ft_perror(MSH, "warning", NULL);
		ft_putstr_fd("shell level (", STDERR_FILENO);
		ft_putnbr_fd(aux, STDERR_FILENO);
		ft_putendl_fd(") too high, resetting to 1", STDERR_FILENO);
		aux = 1;
	}
	else
		aux++;
	return (aux);
}

void	ft_update_shlvl(void)
{
	int		aux;
	char	*shlvl;
	char	*aux_str;

	aux = ft_update_shlvl_aux(&shlvl);
	if (aux == INVALID)
		return ;
	aux_str = ft_itoa(aux);
	shlvl = ft_strjoin2("SHLVL=", aux_str);
	update_env_vars((char *[2]){shlvl, NULL});
	free(shlvl);
}

bool	valid_identifier(char *arg)
{
	size_t	i;

	i = 0;
	if (arg[0] == '_' && arg[1] == '=')
		return (false);
	if (!isalpha(*arg) && *arg != '_')
	{
		export_error(arg);
		return (false);
	}
	while (arg[i] && arg[i] != '=')
	{
		if (ft_isalnum(arg[i]) || arg[i] == '_')
			i++;
		else
		{
			export_error(arg);
			return (false);
		}
	}
	return (true);
}

void	ft_update_pwd(void)
{
	const char	*oldpwd = ft_getenv("OLDPWD");
	char		*aux;
	char		*pwd_data;
	struct stat	dir;

	aux = getcwd(NULL, 0);
	pwd_data = ft_strjoin2("PWD=", aux);
	get_current_dir(aux, true);
	free(aux);
	update_env_vars((char *[2]){pwd_data, NULL});
	free(pwd_data);
	if (oldpwd && stat(oldpwd, &dir) != INVALID)
		return ;
	update_env_vars((char *[2]){"OLDPWD", NULL});
}
