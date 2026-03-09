/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 15:57:56 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:48 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*return_line(t_list **head, char **buffer, ssize_t len)
{
	t_list	*aux;
	char	*line;

	free(*buffer);
	if (len <= 0)
		return (NULL);
	line = ft_malloc(sizeof(char) + (len + 1), General);
	if (!line)
	{
		ft_lstclear(head, free);
		return (NULL);
	}
	len = 0;
	while (head && *head)
	{
		aux = *head;
		*head = (*head)->next;
		line[len++] = *(char *)(aux->content);
		free(aux->content);
		free(aux);
	}
	line[len] = '\0';
	return (line);
}

char	*no_interactive_mode(void)
{
	t_list	*list;
	ssize_t	len;
	char	*buffer;
	int		read_ret;

	len = 0;
	list = NULL;
	while (true)
	{
		buffer = malloc(sizeof(char) + sizeof(char));
		read_ret = read(STDIN_FILENO, buffer, sizeof(char));
		if (read_ret < 0)
		{
			ft_lstclear(&list, free);
			return (free(buffer), NULL);
		}
		buffer[read_ret] = '\0';
		len += read_ret;
		if (read_ret == 0)
			return (return_line(&list, &buffer, len));
		if (*buffer == '\n')
			return (return_line(&list, &buffer, len));
		ft_lstadd_back(&list, ft_lstnew(buffer));
	}
}

char	*get_prompt(void)
{
	char		str[1024];
	static char	*username;

	*str = 0;
	username = ft_getenv("USER");
	if (ft_strlen(username) <= 1020)
	{
		ft_strlcpy(str, "\001\033[32m\002", sizeof(str));
		ft_strlcat(str, username, sizeof(str));
		username = ft_strjoin2(str, "$ \001\033[0m\002");
	}
	else
		username = DEFAULT_PROMPT;
	return (username);
}

char	*get_user_input(void)
{
	char	*username;
	char	*line;

	//if (!isatty(STDIN_FILENO))
		//return (no_interactive_mode());
	username = get_prompt();
	line = readline(username);
	g_sig_re = 0;
	if (ft_strcmp(DEFAULT_PROMPT, username))
		free(username);
	if (line && *line)
		add_history(line);
	else if (line && !*line)
	{
		free(line);
		line = "";
	}
	else
		return (free(line), NULL);
	return (line);
}
