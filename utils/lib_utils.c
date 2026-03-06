/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:47:38 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:45 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	ft_perror(char *at, char *cmd, char *msg)
{
	ft_putstr_fd(at, STDERR_FILENO);
	if (at)
		ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (cmd)
		ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

char	*ft_strdup2(const char *s)
{
	char	*str;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	str = (char *)ft_calloc((len + 1), sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i <= len)
	{
		str[i] = s[i];
		i++;
	}
	return (str);
}

char	*ft_strjoin2(char const *s1, char const *s2)
{
	size_t	size;
	char	*str;
	size_t	len_s1;
	size_t	len_s2;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	size = len_s1 + len_s2;
	str = (char *)ft_calloc((size + 1), sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, len_s1 + 1);
	ft_strlcat(str, s2, size + 1);
	return (str);
}

int	ft_ifs(int c)
{
	if ((c == ' ' || c == '\t'))
		return (1);
	return (0);
}

size_t	strlen_chr(char *str, char limit)
{
	size_t	len;

	len = 0;
	while (str && str[len] && str[len] != limit)
		len++;
	return (len);
}

