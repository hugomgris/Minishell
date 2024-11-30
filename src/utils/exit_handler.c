/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:54:30 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/30 16:47:56 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Small exit handler that prints an exit msg, cleans ms_env and exits.
If needed later on, we can move this to it's own file in utils.
*/
void	ms_exit_handler(t_list **ms_env, t_list **gc, const char *msg)
{
	if (msg)
		ft_printf("%s\n", msg);
	ft_lstclear(ms_env, free);
	ft_lstclear(gc, free);
	exit(0);
}
