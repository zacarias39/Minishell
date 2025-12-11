/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:05:48 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/10 13:01:35 by dadmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	print_error(char *token, char rigor)
{
	static char	*str;

	if (rigor == false)
	{
		if (token)
			str = token;
		return (false);
	}
	if (token)
		str = token;
	printf("sh: syntax error near unexpected token `%s'\n", str);
	ft_strtok(NULL, false, true);
	exit(1);
}

char	*ft_strclose(char *str, int *i)
{
	static int	single_quote;
	static int	double_quote;
	char		*start;
	int			is_on;

	is_on = 0;
	start = &str[*i];
	while (str[*i])
	{
		if (str[*i] == '\"' && !single_quote)
		{
			double_quote = 1 - double_quote;
			is_on = double_quote;
		}
		else if (str[*i] == '\'' && !double_quote)
		{
			single_quote = 1 - single_quote;
			is_on = single_quote;
		}
		if ((ft_strchr(OPERATORS, str[*i]) || ft_isspace(str[*i])) && (is_on
				% 2 == 0))
			break ;
		(*i)++;
	}
	str[(*i)] = EOS;
	return (start);
}

char	*get_operator(char *str, int *i)
{
	char	*start;
	char	is_same;

	while (ft_isspace(str[*i]))
		(*i)++;
	if (!ft_strchr(OPERATORS, str[*i]))
		return (ft_strclose(str, i));
	start = &str[*i];
	is_same = str[*i];
	while (str[*i])
	{
		if (!ft_strchr(OPERATORS, str[*i]) || is_same != str[*i])
			break ;
		(*i)++;
	}
	str[(*i)] = EOS;
	return (start);
}

void	*pointer_clear(char **s1, char **s2, int *times, int *i)
{
	free(*s1);
	free(*s2);
	*s1 = NULL;
	*s2 = NULL;
	*i = 0;
	*times = 0;
	return (NULL);
}

char	*ft_strtok(char *str, char op, char clean)
{
	static char	*str_1;
	static char	*str_2;
	static char	*start;
	static int	times;
	static int	i;

	if (clean == true)
	{
		pointer_clear(&str_2, &str_1, &times, &i);
		return (NULL);
	}
	// Now I don't even check if str_2 is NULL, I just directly strdup it whenever there is a new pointer
	// I did this because after freeing it, instead of pointing to NULL, it was pointing a garbage on the next call
	if (str != NULL)
	{
		str_1 = str;
		str_2 = ft_strdup(&str_1[i]);
	}
	if ((!str_1 || !str_1[i]) && (!str_2 || !str_2[i]) && !op)
		return (start = NULL, NULL);
	if (op == true)
		return (start);
	if (times % 2 == 0)
		start = get_operator(str_1, &i);
	else
		start = get_operator(str_2, &i);
	return (times++, start);
}
