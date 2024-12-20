/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/20 13:23:41 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Universal error handler for Minishell.
Prints error msg sent as argument, checks for critical errors (alloc errors).
If critical error, calls Garbage Collector and exits.
*/
void	ms_error_handler(t_ms *ms, char *msg, int critical)
{
	char	*output;

	output = ft_strjoin("minishell: ", msg);
	gc_add(output, &ms->gc);
	ft_putendl_fd(output, STDERR_FILENO);
	if (critical)
	{
		ft_lstclear(&ms->ms_env, free);
		ft_lstclear(&ms->gc, free);
		ft_lstclear(&ms->tokens, free);
		ms_tokclear(&ms->tok, free);
		exit (1);
	}
}
