/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/18 16:54:02 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Exit handler.
Only non-error exit point for minishell, reached by exit input or ctrl+D.
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
	exit(code);
}

/*
Exit builtin command intermediary.
Checks arguments, handles the exit code, calls handler.
*/
void	ms_exit(t_ms *ms)
{
	int		code;
	t_list	*tokens;

	tokens = ms->exec_tokens[0];
	if (ft_lstsize(tokens) >= 3)
	{
		ms_error_handler(ms, "exit: too many arguments", 0);
		return ;
	}
	else if (ft_lstsize(tokens) == 2)
	{
		if (ft_isdigit_str(tokens->next->content))
			code = ft_atoi(tokens->next->content) % 256;
		else
		{
			ms_error_handler(ms, "exit: numeric argument required", 0);
			return ;
		}
	}
	else
		code = 0;
	ms_exit_handler(ms, "exit", code);
	return ;
}
