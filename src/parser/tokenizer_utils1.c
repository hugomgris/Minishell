/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:49:40 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/17 11:25:20 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_quote(char c)
{
	if (c == 34 || c == 39)
		return (TRUE);
	return (FALSE);
}

int	is_operator(char *c)
{
	if (c[0] == '<' || c[0] == '>' || c[0] == '|' || c[0] == '&'
		|| c[0] == '(' || c[0] == ')' || (c[0] == '2' && c[1] == '>'))
		return (TRUE);
	return (FALSE);
}

void	ms_skip_space(char **str)
{
	while (**str && ft_isspace(**str))
		(*str)++;
}

int	ms_skip_quotes(t_ms *ms, char *str, int *i)
{
	char	quote;

	quote = str[*i];
	if (ft_strchr(str + *i + 1, quote))
	{
		(*i)++;
		while (str[*i] != quote)
			(*i)++;
		(*i)++;
		return (TRUE);
	}
	else
	{
		ms_error_handler(ms, "Error: Invalid quote", 0);
		return (FALSE);
	}
}

/*
	TODO
	Check and protect against wrong parenthesis: doubles, unclosed, etc.
*/

int	ms_check_operator(t_ms *ms, char **str)
{
	if (!ft_strncmp(*str, "<<<", 3))
	{
		ms_error_handler(ms, "Syntax error near unexpected token `<<'", 0);
		return (TRUE);
	}
	if (!ft_strncmp(*str, ">>>", 3))
	{
		ms_error_handler(ms, "Syntax error near unexpected token `>>'", 0);
		return (TRUE);
	}
	if (!ft_strncmp(*str, "|||", 3))
	{
		ms_error_handler(ms, "Parse error near unexpected token `|'", 0);
		return (TRUE);
	}
	if (!ft_strncmp(*str, "&&&", 3))
	{
		ms_error_handler(ms, \
			"minishell: Parse error near unexpected token `&'", 0);
		return (TRUE);
	}
	return (FALSE);
}
