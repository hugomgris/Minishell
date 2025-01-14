/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/13 16:47:46 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	
	Checks for special characters that are not handled by the minishell
	('\', ';').
*/
int	ms_checkspecialchar(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == 59 || str[i] == 92 || str[i] == 10)
		{
			ms_error_handler(ms, "invalid character: ';' '\\' '\\n'\n", 0);
			return (FALSE);
		}
	}
	return (TRUE);
}

/*
	Checks for an empty pipe, ie. a syntax with "| NULL |" or
	"cmd | NULL".
*/
int	ms_check_empty_pipe(t_ms *ms, char *str)
{
	str++;
	while (ft_isspace(*str))
		str++;
	if (*str == '|')
	{
		ms_error_handler(ms, "syntax error near unexpected token \"|\"", 0);
		return (TRUE);
	}
	else if (*str == '\0')
	{
		ms_error_handler(ms, "syntax error near unexpected token \"|\"", 0);
		return (TRUE);
	}
	else
		return (FALSE);
}

/*
	Checks for incorrect pipe syntax, such as " | command1" or "command1 | ".
*/
int	ms_checkpipes(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (ft_isspace(str[++i]))
		;
	if (str[i] == '|')
	{
		ms_error_handler(ms, "syntax error near unexpected token \"|\"", 0);
		return (FALSE);
	}
	while (str[i++])
	{
		if (str[i] == '|' && str[i + 1] == '|')
			i++;
		else if (str[i] == '|')
		{
			if (ms_check_empty_pipe(ms, str + i))
				return (FALSE);
		}
	}
	return (TRUE);
}

/*
	Checks for incorrect parenthesis syntax by substracting the number
	of closed parenthesis to the number of open ones, ignoring quotes.
	Non-zero result indicates a wrong syntax and raises error.
*/
int	ms_check_parenthesis(t_ms *ms, char *str)
{
	int	i;
	int	open;

	i = -1;
	open = 0;
	while (str[++i])
	{
		if (ms_is_quote(str[i]))
		{
			if (!ms_skip_quotes(ms, str, &i))
				return (FALSE);
			--i;
		}
		if (str[i] == '(')
			open++;
		if (str[i] == ')')
			open--;
	}
	if (!open)
		return (TRUE);
	else if (open > 0)
		ms_error_handler(ms, "syntax error near unexpected token \"(\"", 0);
	else
		ms_error_handler(ms, "syntax error near unexpected token \")\"", 0);
	return (FALSE);
}

/*
	Checks for special characters that are not handled by minishell along
	with the correct syntax for using pipes and redirections.
	Returns error in case of bad syntax, displaying a new prompt to the user.
*/
int	ms_syntax_checker(t_ms *ms, char *str)
{
	if (!ms_checkspecialchar(ms, str))
		return (FALSE);
	if (!ms_check_and(ms, str))
		return (FALSE);
	if (!ms_check_or(ms, str))
		return (FALSE);
	if (!ms_checkpipes(ms, str))
		return (FALSE);
	if (!ms_check_parenthesis(ms, str))
		return (FALSE);
	return (TRUE);
}
