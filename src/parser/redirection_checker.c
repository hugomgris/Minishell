/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by nponchon          #+#    #+#             */
/*   Updated: 2024/11/25 11:55:56 by nponchon         ###   ########.fr       */
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
		if (str[i] == '>' && str[i + 1] == '>')
		{
			// call opening function in corresponding mode;
			ft_printf("Redirect output to: s%\n", ft_strtok(str + 2, " "));
			i += 2;
		}
		else if (str[i] == '>' && ft_strtok(str + 1, " "))
			ft_printf("Redirect output to: s%\n", ft_strtok(str + 2, " "));
	}
	return (TRUE);
}

/*  TODO
    Use a flag to mark in the data struct that here_doc is necessary and/or
    call here_doc initialiser directly, same with infile.
*/

int	ms_checkinfile(char *str)
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
			ms_error_handler(NULL, "Wrong infile", FALSE);
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
			;
		}
	}
	return (TRUE);
}

int	ms_checkredirections(char *str)
{
	if (!ms_checkinfile(str))
		return (FALSE);
	if (!ms_checkoutfile(str))
    	return (FALSE);
	return (TRUE);
}
