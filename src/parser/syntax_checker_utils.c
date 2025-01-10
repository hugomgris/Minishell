/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:41:32 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/10 16:41:56 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	{
		if (!ft_strncmp(str + i, "&&", 2))
		{
			i += 2;
			while (ft_isspace(str[i]))
				i++;
			if (ms_is_doubleoperator(ms, str, i))
				return (FALSE);
		}
	}
	return (TRUE);
}

int	ms_check_or(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (ft_isspace(str[++i]))
		;
	if (!ft_strncmp(str + i, "||", 2))
	{
		ms_error_handler(ms, "syntax error near unexpected token \"&&\"", 0);
		return (FALSE);
	}
	while (str[i++])
	{
		if (!ft_strncmp(str + i, "||", 2))
		{
			i += 2;
			while (ft_isspace(str[i]))
				i++;
			if (ms_is_doubleoperator(ms, str, i))
				return (FALSE);
		}
	}
	return (TRUE);
}
