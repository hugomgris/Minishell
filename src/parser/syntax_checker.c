/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/13 12:46:16 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	
	TODO
	- Check for pipes (no pipe w/o command before or after)
	- Check for special characters ('\', '\n', ';' etc.)
*/

int	ms_checkspecialchar(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == 59 || str[i] == 92)
		{
			ms_error_handler(ms, "Invalid character: ';' '\\'\n", 1);
			return (FALSE);
		}
	}
	return (TRUE);
}

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
