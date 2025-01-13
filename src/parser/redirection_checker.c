/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checker.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/13 16:48:26 by nponchon         ###   ########.fr       */
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

	i = 0;
	while (str[i])
	{
		if (ms_is_redirection(str[i]) && ms_is_redirection(str[i + 1]))
			i++;
		if (!str[i + 1])
		{
			ms_error_handler(ms, \
				"syntax error near redirection operator", 0);
			return (FALSE);
		}
		i++;
	}
	return (TRUE);
}
