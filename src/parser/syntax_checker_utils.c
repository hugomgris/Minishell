/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:41:32 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/13 10:17:06 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Return TRUE if c is an i/o redirection character, FALSE otherwise.
*/
int	ms_is_redirection(char c)
{
	if (c == '>' || c == '<')
		return (TRUE);
	return (FALSE);
}

/*
	Returns TRUE if the current index in the string is on a double operator.
	FALSE otherwise.
*/
int	ms_is_doubleoperator(t_ms *ms, char *str, int i)
{
	if (!ft_strncmp(str + i, "&&", 2))
	{
		ms_error_handler(ms, "syntax error near unexpected token \"&&\"", 0);
		return (TRUE);
	}
	else if (!ft_strncmp(str + i, "||", 2))
	{
		ms_error_handler(ms, "syntax error near unexpected token \"||\"", 0);
		return (TRUE);
	}
	return (FALSE);
}

/*
	Checks for empty double operator command, eg.
	'cmd1 || && cmd2'.
*/
int	ms_handle_andor_error(t_ms *ms, char *str, int i)
{
	i += 2;
	while (ft_isspace(str[i]))
		i++;
	if (ms_is_doubleoperator(ms, str, i))
		return (FALSE);
	return (TRUE);
}

/*
	Checks for correct syntax of OR operator (&&).
	Unlike bash, minishell does not ask user for complete input
	in case of missing command ("CMD1 && ").
*/
int	ms_check_and(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (ft_isspace(str[++i]))
		;
	if (!ft_strncmp(str + i, "&&", 2))
	{
		ms_error_handler(ms, "syntax error near unexpected token \"&&\"", 0);
		return (FALSE);
	}
	while (str[i++])
		if (!ft_strncmp(str + i, "&&", 2) && !ms_handle_andor_error(ms, str, i))
			return (FALSE);
	if (!ft_strncmp(str + i - 3, "&&", 2))
	{
		ms_error_handler(ms, "syntax error near unexpected token \"&&\"", 0);
		return (FALSE);
	}
	return (TRUE);
}

/*
	Checks for correct syntax of OR operator (||).
	Unlike bash, minishell does not ask user for complete input
	in case of missing command ("CMD1 || ").
*/
int	ms_check_or(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (ft_isspace(str[++i]))
		;
	if (!ft_strncmp(str + i, "||", 2))
	{
		ms_error_handler(ms, "syntax error near unexpected token \"||\"", 0);
		return (FALSE);
	}
	while (str[i++])
		if (!ft_strncmp(str + i, "||", 2) && !ms_handle_andor_error(ms, str, i))
			return (FALSE);
	if (!ft_strncmp(str + i - 3, "||", 2))
	{
		ms_error_handler(ms, "syntax error near unexpected token \"||\"", 0);
		return (FALSE);
	}
	return (TRUE);
}
