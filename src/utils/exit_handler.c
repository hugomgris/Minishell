/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:54:30 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/09 11:13:51 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_list(void *content)
{
	ft_printf("%s\n", (char *)content);
}

/*
Small exit handler that prints an exit msg, cleans ms_env and exits.
If needed later on, we can move this to it's own file in utils.
*/
void	ms_exit_handler(t_ms *ms, const char *msg)
{
	if (msg)
		ft_printf("%s\n", msg);
	ft_lstclear(&ms->ms_env, free);
	ft_lstclear(&ms->gc, free);
	ft_lstclear(&ms->tokens, free);
	exit(0);
}
