/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/02 18:23:36 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Exit handler.
Only non-error exit point for minishell, reached by "exit" input or ctrl+D.
Prints exit string ("exit"), cleans structs and memory.
Sets an exit code if provided (checkable with echo $?)
*/
void	ms_exit_handler(t_ms *ms, const char *msg, int code)
{
	if (msg)
		ft_printf("%s\n", msg);
	ft_lstclear(&ms->ms_env, free);
	ft_lstclear(&ms->gc, free);
	if (ft_lstsize(ms->tokens))
		ft_lstclear(&ms->tokens, free);
	ms_tokclear(&ms->tok, free);
	exit(code);
}

/*
Exit builtin command intermediary function.
Checks arguments, handles the exit code, calls handler.
*/
int	ms_exit(t_ms *ms)
{
	int		code;

	code = 0;
	if (ft_array_count(ms->filt_args) >= 3)
	{
		ms_error_handler(ms, "exit: too many arguments", 0);
		return (1);
	}
	if (ms->filt_args[1])
	{
		if (ft_isdigit_str(ms->filt_args[1]))
			code = ft_atoi(ms->filt_args[1]) % 256;
		else
		{
			ms_error_handler(ms, "exit: numeric argument required", 0);
			return (1);
		}
	}
	ms_exit_handler(ms, "exit", code);
	return (0);
}
