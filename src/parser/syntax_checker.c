/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/17 15:32:25 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	
	Checks for special characters that are not handled by the minishell
	('\', '\n', ';' etc.).
*/
int	ms_checkspecialchar(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == 59 || str[i] == 92)
		{
			ms_error_handler(ms, "Invalid character: ';' '\\'\n", 0);
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
		ms_error_handler(ms, "Wrong pipe: empty pipe \"|\"", 0);
		return (TRUE);
	}
	else if (*str == '\0')
	{
		ms_error_handler(ms, "Wrong pipe: no command after \"|\"", 0);
		return (TRUE);
	}
	else
		return (FALSE);
}

/*
	Checks for an empty pipe, ie. a syntax with "| NULL |" or
	"cmd | NULL".
*/
int	ms_checkpipes(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (ft_isspace(str[++i]))
		;
	if (str[i] == '|')
	{
		ms_error_handler(ms, "Wrong pipe: no command before \"|\"", 0);
		return (FALSE);
	}
	while (str[i++])
	{
		if (str[i] == '|')
		{
			if (ms_check_empty_pipe(ms, str + i))
				return (FALSE);
		}
	}
	return (TRUE);
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
	if (!ms_checkpipes(ms, str))
		return (FALSE);
	if (!ms_checkredirections(ms, str))
		return (FALSE);
	return (TRUE);
}
