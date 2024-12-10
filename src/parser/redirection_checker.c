/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checker.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/10 14:00:16 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*  TODO
    Check for the correct redir syntax and open the file descriptor in the
    corresponding mode (truncate or append). Multiple outfiles are to handle.
*/

int	ms_checkoutfile(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (ft_strnstr(str + i, ">", 1))
		{
			ft_printf("Redirect output to: %s\n", str + i);
			i++;
		}
		else if (ft_strnstr(str + i, ">>", 2))
			ft_printf("Redirect output to: %s\n", str + i);
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
	int	here_doc;
	int	infile;

	i = -1;
	here_doc = FALSE;
	infile = FALSE;
	while (str[++i])
	{
		if (str[i] == '<' && here_doc)
		{
			ms_error_handler(ms, "Wrong infile", 0);
			return (FALSE);
		}
		if (str[i] == '<' && str[i + 1] == '<')
		{
			here_doc = TRUE;
			i++;
		}
		else if (str[i] == '<')
			infile = TRUE;
		if (infile)
		{
			cucufu(1);
		}
	}
	return (TRUE);
}

int	ms_checkredirections(t_ms *ms, char *str)
{
	if (!ms_checkinfile(ms, str))
		return (FALSE);
	if (!ms_checkoutfile(str))
		return (FALSE);
	return (TRUE);
}
