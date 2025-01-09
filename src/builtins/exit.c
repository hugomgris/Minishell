/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/09 10:52:23 by hmunoz-g         ###   ########.fr       */
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
	if (ms_toksize(ms->tok))
		ms_tokclear(&ms->tok, free);
	exit(code);
}

/*
Exit builtin command intermediary function.
Checks arguments, handles the exit code, calls handler.
*/

int	ms_exit(t_ms *ms)
{
	long long	code;
	char		*endptr;

	code = 0;
	if (ft_array_count(ms->filt_args) >= 3)
		return (ms_error_handler(ms, "exit: too many arguments", 0), 1);
	if (ms->filt_args[1])
	{
		errno = 0;
		code = ft_strtoll(ms->filt_args[1], &endptr);
		if (!ft_isdigit_str(ms->filt_args[1]) || errno == ERANGE
			|| *endptr != '\0')
			return (ms_error_handler(ms, "exit: numeric argument required", 0),
				ms_exit_handler(ms, "exit", 2), 2);
		code = (code % 256 + 256) % 256;
	}
	return (ms_exit_handler(ms, "exit", code), 0);
}
