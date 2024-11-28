/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:54:30 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 12:54:41 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Small exit handler that prints an exit msg, cleans ms_env and exits.
If needed later on, we can move this to it's own file in utils.
*/
void	ms_exit_handler(const char *msg, t_list *ms_env)
{
	if (msg)
		ft_printf("%s\n", msg);
	ft_lstclear(&ms_env, free); //Add GC
	exit(0);
}
