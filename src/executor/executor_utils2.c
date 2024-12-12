/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/12 20:22:12 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to check if an input command exists. 
It is used to end execution if an invalid command is input.
*/
char	*validate_command(t_ms *ms)
{
	char	*path;

	path = ms_get_command_path(ms, ms->tokens->content);
	if (!path)
		ms_error_handler(ms, "Command not found", 0);
	return (path);
}

/*
Helper function to clear the execution tools (env array for children, tokens).
*/
void	ms_executor_cleanup(t_ms *ms, char **arr)
{
	free(arr);
	ft_lstclear(&ms->tokens, free);
	ms_get_set(1, 0);
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
