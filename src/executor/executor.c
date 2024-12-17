/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/16 14:47:14 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to clear the execution tools (env array for children, tokens).
*/
void	ms_executor_cleanup(t_ms *ms, char **arr, int stdout_b, int stdin_b)
{
	ft_free(arr);
	dup2(stdout_b, STDOUT_FILENO);
	dup2(stdin_b, STDIN_FILENO);
	close(stdout_b);
	close(stdin_b);
	ft_lstclear(&ms->tokens, free);
	ft_lstclear(&ms->filtered_tokens, free);
}

char	**ms_prepare_execution(t_ms *ms, char **arr)
{
	ms->filtered_tokens = ms_filter_tokens(ms->tokens);
	arr = ms_list_to_array(ms, arr);
	if (!arr)
	{
		ms_error_handler(ms, "Failed to prepare environment", 0);
		return (NULL);
	}
	return (arr);
}

int	ms_handle_system_command(t_ms *ms, char **arr)
{
	char	*path;
	pid_t	pid;
	int		status;

	path = ms_validate_command(ms);
	if (!path)
		return (0);
	gc_add(path, &ms->gc);
	pid = fork();
	if (pid == -1)
	{
		ms_error_handler(ms, "Fork failed", 0);
		return (0);
	}
	else if (pid == 0)
		execute_child(ms, arr);
	else
		execute_parent(ms, pid, &status);
	return (1);
}

/*
Executor hub.
Makes some checks to know if it needs to execute a builtin or a system cmd.
Creates forks for child processes, calls for execution of both child and parent.
Calls the executor cleanup.
*/
void	ms_executor(t_ms *ms)
{
	int		stdout_b;
	int		stdin_b;
	char	**arr;

	stdout_b = dup(STDOUT_FILENO);
	stdin_b = dup(STDIN_FILENO);
	arr = (char **)malloc(sizeof(char *) * (ft_lstsize(ms->ms_env) + 1));
	if (!arr)
		return ;
	arr = ms_prepare_execution(ms, arr);
	if (!arr)
		return ;
	if (ms_has_redirection(ms))
	{
		if (ms_redirection(ms) == -1)
		{
			ms_executor_cleanup(ms, arr, stdout_b, stdin_b);
			return ;
		}
	}
	if (ms_is_builtin(ms->filtered_tokens->content))
		ms_execute_builtin(ms);
	else
		ms_handle_system_command(ms, arr);
	ms_executor_cleanup(ms, arr, stdout_b, stdin_b);
}
