/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checker.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/11 16:48:35 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*  TODO
    Check for the correct redir syntax and open the file descriptor in the
    corresponding mode (truncate or append). Multiple outfiles are to handle.
*/

int	ms_checkoutfile(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '>' && !str[i + 1])
		{
			ms_error_handler(ms, \
				"syntax error near unexpected token `newline'", 0);
			return (FALSE);
		}
	}
	return (TRUE);
}

/*  TODO
    Use a flag to mark in the data struct that here_doc is necessary and/or
    call here_doc initialiser directly, same with infile.
*/

int	ms_checkinfile(t_ms *ms, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '<' && !str[i + 1])
		{
			ms_error_handler(ms, \
				"syntax error near unexpected token `newline'", 0);
			return (FALSE);
		}
	}
	return (TRUE);
}

int	ms_checkredirections(t_ms *ms, char *str)
{
	if (!ms_checkinfile(ms, str))
		return (FALSE);
	if (!ms_checkoutfile(ms, str))
		return (FALSE);
	return (TRUE);
}
