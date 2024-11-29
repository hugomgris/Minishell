/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 16:00:36 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Universal error handler for Minishell.
Prints error msg sent as argument, checks for critical errors (alloc errors).
If critical error, calls Garbage Collector and exits.
*/
void	ms_error_handler(char *msg, int is_critical)
{
	ft_putendl_fd(msg, STDERR_FILENO);
	if (is_critical)
	{
		gc_cleanup();
		exit (1);
	}
}
