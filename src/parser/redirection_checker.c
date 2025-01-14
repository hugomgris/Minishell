/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checker.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/13 12:50:26 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
    Checks for a correct usage of the redirection operator, yields an error
	in case it finds an operator without a filename.
*/
int	ms_checkiofile(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (ms_is_redirection(str[i]) && ms_is_redirection(str[i + 1]))
			i++;
		if (!str[i + 1])
		{
			ms_error_handler(ms, \
				"syntax error near redirection operator", 0);
			return (FALSE);
		}
		else if (str[i + 1])
		{
			i++;
			while (ft_isspace(str[i]))
				i++;
			if (ms_is_redirection(str[i]))
			{
				ms_error_handler(ms, \
					"syntax error near redirection operator", 0);
				return (FALSE);
			}
		}
	}
	return (TRUE);
}

int	ms_check_emptyparenthesis(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '(')
		{
			i++;
			while (ft_isspace(str[i]))
				i++;
			if (str[i] == ')')
			{
				ms_error_handler(ms, \
					"syntax error: empty parenthesis", 0);
				return (FALSE);
			}
		}
	}
	return (TRUE);
}

/*
	Checks for a redirection after or before parenthesis, which minishell must
	not handle.
*/
/*
int	ms_check_redirparenthesis(t_ms *ms, char *str)
{
	int	i;

	i = 0;
	while (str[i] && !ms_is_redirection(str[i]))
		i++;
	if (str[i] && ms_is_redirection(str[i + 1]))
		i++;
	if (str[i])
	{
		i++;
		while (ft_isspace(str[i]))
			i++;
		if (str[i] == '(')
		{
			ms_error_handler(ms, \
				"error: unsupported redirection with parenthesis", 0);
			return (FALSE);
		}
	}
	return (TRUE);
}
*/

/*
	Does a check of possible i/o redirections, returns NULL in case of bad input.
	TODO
	Following syntaxes must not be handled:
	- ()
	- (...) >
	- < (...)
*/
int	ms_checkredirections(t_ms *ms, char *str)
{
	//if (!ms_checkiofile(ms, str))
	//	return (FALSE);
	if (!ms_check_emptyparenthesis(ms, str))
		return (FALSE);
	return (TRUE);
}
