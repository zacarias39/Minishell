/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:05:48 by dadmendo          #+#    #+#             */
/*   Updated: 2026/03/10 13:03:56 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

char	on_error(char *token, char rigor)
{
	static char	*str;
	static char	error;

	if (token)
		str = token;
	else
		str = "newline";
	if (rigor == CHECK)
		return (error);
	if (rigor == RESET)
	{
		error = false;
		ft_malloc(0, TreeFree);
		return (false);
	}
	if (rigor == false || error == true)
		return (false);
	last_cmd_status(2, UPDATE_DATA);
	if (rigor == RAISE)
		return (error = true, false);
	printf(ERROR_MSG "`\e[31m%s\e[0m'\n", str);
	return (error = true, false);
}

int	ft_strclose(char *str, int i)
{
	char	quotes;

	quotes = 0;
	while (str[i])
	{
		if (!quotes && (str[i] == '\"' || str[i] == '\''))
			quotes = str[i];
		else if (quotes == str[i])
			quotes = 0;
		if ((ft_strchr(OPERATOR_BONUS, str[i]) || ft_ifs(str[i])) && !quotes)
			break ;
		i++;
	}
	return (i);
}

void	get_token(char *str, int *i)
{
	static char	**operators;
	int			index;
	int			len;

	index = -1;
	if (operators == NULL)
		operators = ft_split(OPERATORS_BONUS, ' ');
	if (!ft_strchr(OPERATOR_BONUS, str[*i]))
		return (*i = ft_strclose(str, *i), (void) NULL);
	if (str[*i] == '&' && str[*i + 1] != '&')
		return (*i += 1, (void) NULL);
	while (operators && operators[++index])
	{
		len = ft_strlen(operators[index]);
		if (!ft_strncmp(&str[*i], operators[index], len))
			return (*i += len, (void) NULL);
	}
}

char	*get_next_token(char *str, int *i)
{
	static char	removed;
	char		*start;

	if (*i == 0)
		removed = str[*i];
	if (removed)
		str[*i] = removed;
	while (ft_ifs(str[*i]))
		(*i)++;
	if (!str[*i])
		return (NULL);
	start = &str[*i];
	get_token(str, i);
	removed = str[*i];
	str[*i] = EOS;
	if (!ft_strchr(OPERATOR_BONUS, *start))
		return (ft_strdup(start));
	return (start);
}

char	*ft_strtok(char *str, char option, char clean)
{
	static char	*input;
	static char	*start;
	static int	len;
	static int	i;

	if (clean == true)
		return (free(input), NULL);
	if (str != NULL)
	{
		i = 0;
		input = str;
		len = ft_strlen(str);
	}
	if (option == PREVIOUS)
		return (start);
	if ((len == i && option == NEXT) || !input)
		return (start = NULL, NULL);
	start = get_next_token(input, &i);
	return (start);
}
