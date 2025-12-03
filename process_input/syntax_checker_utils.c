/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:40:01 by zcasimir          #+#    #+#             */
/*   Updated: 2025/12/02 13:42:16 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// EXPECTS THE TOKEN TO BE THE SAME AS THE EXPECTED STRING
bool	expect(char *expected, bool rigor)
{
	char	*token;

	token = ft_strtok(NULL, true, false);
	if (ft_strcmp(expected, token))
		return (print_error(token, rigor));
	ft_strtok(NULL, false, false);
	return (true);
}

char	expect_word(char *token, char rigor)
{
	static int	double_quote;
	static int	single_quote;
	static int	is_on;
	int			i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '\'' && !double_quote)
		{
			single_quote = 1 - single_quote;
			is_on = single_quote;
		}
		else if (token[i] == '\"' && !single_quote)
		{
			double_quote = 1 - double_quote;
			is_on = double_quote;
		}
		if (is_on && token[i + 1] == '\0')
			return (print_error(token, true));
		if (ft_strchr(OPERATORS, token[i]) && !is_on)
			return (print_error(token, rigor));
		i++;
	}
	return (true);
}

void	*get_list(bool is_check, int *is_true)
{
	t_wordlist	*list;
	char		*token;
	int			i;

	i = 0;
	list = NULL;
	token = ft_strtok(NULL, true, false);
	while (token != NULL)
	{
		if (expect_word(token, false) == false)
			break ;
		if (is_check == false)
			list_add(&list, token);
		token = ft_strtok(NULL, false, false);
		i++;
	}
	if (i >= 1)
		*is_true = true;
	return (list);
}