/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/30 15:26:08 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Universal error handler for Minishell.
Prints error msg sent as argument, checks for critical errors (alloc errors).
If critical error, calls Garbage Collector and exits.
*/
void	ms_error_handler(t_list **ms_env, t_list **gc, char *msg, int critical)
{
	ft_putendl_fd(msg, STDERR_FILENO);
	if (critical)
	{
		ft_lstclear(ms_env, free);
		ft_lstclear(gc, free);
		exit (1);
	}
}
