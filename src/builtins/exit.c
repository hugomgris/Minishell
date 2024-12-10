/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/10 17:21:51 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Small exit handler that prints an exit msg, cleans ms_env and exits.
If needed later on, we can move this to it's own file in utils.
*/
void	ms_exit_handler(t_ms *ms, const char *msg, int code)
{
	if (msg)
		ft_printf("%s\n", msg);
	ft_lstclear(&ms->ms_env, free);
	ft_lstclear(&ms->gc, free);
	//ft_lstclear(&ms->tokens, free);
	exit(code);
}

void	ms_exit(t_ms *ms, char **args)
{
	int		code;

	if (ft_array_count(args) >= 3)
	{
		ms_error_handler(ms, "exit: too many arguments", 0);
		return ;
	}
	else if (ft_array_count(args) == 2)
	{
		if (ft_isdigit_str(args[1]))
			code = ft_atoi(args[1]) % 256;
		else
		{
			ms_error_handler(ms, "exit: numeric argument required", 0);
			return ;
		}
	}
	else
		code = 0;
	ms_exit_handler(ms, "exit", code);
	ft_free(args);
	return ;
}
