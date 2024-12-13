/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/13 10:30:49 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to clear the execution tools (env array for children, tokens).
*/
void	ms_executor_cleanup(t_ms *ms, char **arr)
{
	free(arr);
	ft_lstclear(&ms->tokens, free);
}

/*
Control flow function that branches builtin execution to each of the
	builtin commands' specific handler functions.
*/
void	ms_execute_builtin(t_ms *ms)
{
	if (!ft_strncmp(ms->tokens->content, "cd", 2))
	{
		if (!ms->tokens->next
			|| ((char *)(ms->tokens->next->content))[0] == ' ')
			ms_cd(ms, NULL);
		else if (ms->tokens->next && !ms->tokens->next->next)
			ms_cd(ms, ms->tokens->next->content);
		else
			ms_error_handler(ms, "cd: invalid args", 0);
	}
	else if (!ft_strncmp(ms->tokens->content, "env", 3))
		ms_env(ms);
	else if (!ft_strncmp(ms->tokens->content, "pwd", 3))
		ms_pwd(ms);
	else if (!ft_strncmp(ms->tokens->content, "unset", 5))
		ms_unset(ms, ms->tokens);
	else if (!ft_strncmp(ms->tokens->content, "exit", 4))
		ms_exit(ms, ms->tokens);
	else if (!ft_strncmp(ms->tokens->content, "echo", 4))
		ms_echo(ms->tokens);
	else if (!ft_strncmp(ms->tokens->content, "export", 6))
		ms_export(ms, ms->tokens);
}

/*
Helper function to check if input comand is a Minishell builtin.
*/
int	ms_is_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "cd", 2)
		|| !ft_strncmp(cmd, "env", 3)
		|| !ft_strncmp(cmd, "pwd", 3)
		|| !ft_strncmp(cmd, "unset", 5)
		|| !ft_strncmp(cmd, "exit", 4)
		|| !ft_strncmp(cmd, "echo", 4)
		|| !ft_strncmp(cmd, "export", 6));
}
