/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 17:02:24 by nponchon         ###   ########.fr       */
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
		ft_putstr_fd("Error: unclosed quote\n", 2);
		return (0);
	}
	return (1);
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
			return (0);
		}
	}
	return (1);
}

int	ms_check_empty_pipe(char *str)
{
	str++;
	while (ft_isspace(*str))
	{
		str++;
	}
	if (*str == '|')
		return (1);
	else if (*str == '\0')
	{
		ms_error_handler("Wrong pipe: no command after \"|\"", 0);
		return (1);
	}
	else
		return (0);
}

/*	Note: even though the input is trimmed, there can be tabs at the
	head and/or tail of the string, hence the ft_isspace()	*/

int	ms_checkpipes(char *str)
{
	int	i;

	i = -1;
	while (ft_isspace(str[++i]))
		;
	if (str[i] == '|')
	{
		ms_error_handler("Wrong pipe: no command before \"|\"", 0);
		return (0);
	}
	while (str[i++])
	{
		if (str[i] == '|')
		{
			if (ms_check_empty_pipe(str + i))
			{
				ms_error_handler("Wrong pipe: empty pipe \"|\"", 0);
				return (0);
			}
		}
	}
	return (1);
}

/*	Each check function returns in case of invalid syntax so as to avoid
	printing multiple error msgs	*/

void	ms_syntax_checker(char *str)
{
	if (!ms_checkquotes(str, 39))
		return ;
	if (!ms_checkquotes(str, 34))
		return ;
	if (!ms_checkpipes(str))
		return ;
	if (!ms_checkspecialchar(str))
		return ;
}
