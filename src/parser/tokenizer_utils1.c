/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:49:40 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/13 12:11:45 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Returns TRUE if the character is a single or a double quote,
	returns FALSE otherwise.
*/
int	ms_is_quote(char c)
{
	if (c == 34 || c == 39)
		return (TRUE);
	return (FALSE);
}

/*
	Returns TRUE if the character pointed to (or the two characters)
	is an operator, returns FALSE otherwise.
*/
int	ms_is_operator(char *c)
{
	if (c[0] == '<' || c[0] == '>' || c[0] == '|' || c[0] == '&'
		|| c[0] == '(' || c[0] == ')' || (c[0] == '2' && c[1] == '>'))
		return (TRUE);
	return (FALSE);
}

/*
	Advances the pointer to a string as long as it contains whitespaces.
	Useful to call from other function by sending the & of a string.
*/
void	ms_skip_space(char **str)
{
	while (**str && ft_isspace(**str))
		(*str)++;
}

/*
	Iterates over a string skipping quotes until it reaches the closing
	quote or returns a FALSE and yields an error if the string
	is not properly closed.
*/
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
		ms_error_handler(ms, "Invalid quote", 0);
		return (FALSE);
	}
}

/*
	Does a check of all possible operators, excluding the ones not
	handled by minishell like herestrings etc.
*/
int	ms_check_operator(t_ms *ms, char **str)
{
	if (!ft_strncmp(*str, "<<<", 3))
	{
		ms_error_handler(ms, "syntax error near unexpected token '<<<'", 0);
		return (TRUE);
	}
	if (!ft_strncmp(*str, ">>>", 3))
	{
		ms_error_handler(ms, "syntax error near unexpected token '>>'", 0);
		return (TRUE);
	}
	if (!ft_strncmp(*str, "|||", 3))
	{
		ms_error_handler(ms, "syntax error near unexpected token '|'", 0);
		return (TRUE);
	}
	if (!ft_strncmp(*str, "&&&", 3))
	{
		ms_error_handler(ms, "syntax error near unexpected token '&'", 0);
		return (TRUE);
	}
	return (FALSE);
}
