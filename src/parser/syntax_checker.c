/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/04 10:39:22 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	TODO

	- Check for quotes (opened and closed)
	Enclosing characters in single-quotes ( '' ) shall preserve
	the literal value of each character within the single-quotes. 
	A single-quote cannot occur within single-quotes.

	- Check for pipes (no pipe w/o command before or after)
	
	- Check for special characters ('\', '\n', ';' etc.)
*/

int	ms_checkquotes(char *str, char c)
{
	int	i;
	int	flag;

	i = -1;
	flag = 0;
	while (str[++i])
	{
		if (str[i] == c && flag == 1)
			flag = 0;
		else if (str[i] == c && flag == 0)
			flag = 1;
	}
	if (flag)
	{
		ms_error_handler(NULL, "Error: Unclosed quotes", 0);
		return (FALSE);
	}
	return (TRUE);
}

int	ms_checkspecialchar(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == 59 || str[i] == 92)
		{
			ft_putstr_fd("Invalid character: ';' '\\'\n", 2);
			return (FALSE);
		}
	}
	return (TRUE);
}

int	ms_check_empty_pipe(t_ms *ms, char *str)
{
	str++;
	while (ft_isspace(*str))
	{
		str++;
	}
	if (*str == '|')
		return (TRUE);
	else if (*str == '\0')
	{
		ms_error_handler(ms, "Wrong pipe: no command after \"|\"", 0);
		return (TRUE);
	}
	else
		return (FALSE);
}

/*	Note: even though the input is trimmed, there can be tabs at the
	head and/or tail of the string, hence the ft_isspace()	*/

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
			{
				ms_error_handler(ms, "Wrong pipe: empty pipe \"|\"", 0);
				return (FALSE);
			}
		}
	}
	return (TRUE);
}

/*	Each check function returns in case of invalid syntax so as to avoid
	printing multiple error msgs	*/

int	ms_syntax_checker(t_ms *ms, char *str)
{
	if (!ms_checkquotes(str, 39))
		return (FALSE);
	if (!ms_checkquotes(str, 34))
		return (FALSE);
	if (!ms_checkpipes(ms, str))
		return (FALSE);
	if (!ms_checkspecialchar(str))
		return (FALSE);
	if (!ms_checkredirections(ms, str))
		return (FALSE);
	if (!ms_checkredirections(str))
		return (FALSE);
	return (TRUE);
}
