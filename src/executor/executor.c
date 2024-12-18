/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/18 10:53:17 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to clear the execution tools (children's env array, tokens).
Returns fd's to their initial values (default: stdin/stdout).
*/
void	ms_executor_cleanup(t_ms *ms, char **arr, int stdout_b, int stdin_b)
{
	ft_free(arr);
	dup2(stdout_b, STDOUT_FILENO);
	dup2(stdin_b, STDIN_FILENO);
	close(stdout_b);
	close(stdin_b);
	ft_lstclear(&ms->tokens, NULL);
	ft_lstclear(&ms->exec_tokens, free);
	ft_lstclear(&ms->redir_tokens, free);
	ft_lstclear(&ms->pipe_tokens, free);
}

/*
Helper function to filter tokens and rebuild env array.
Token filtering is handled by secondary helper function.
*/
char	**ms_prepare_execution(t_ms *ms, char **arr)
{
	ms_filter_tokens(ms);
	arr = ms_env_to_array(ms, arr);
	if (!arr)
	{
		ms_error_handler(ms, "Failed to prepare environment", 0);
		return (NULL);
	}
	return (arr);
}

/*
Flow control function that handles call to system commands.
Calls helper functions to retrieve a cmd path.
Forks processes and sets up the child and parent execution.
*/
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
Goes through different steps:
	-Calls function to prepare exec (to divide redir from exec tokens).
	-Checks if there's need for redirection handling
	-Checks if input contains system or builtin comands.
	-Calls the function that cleans the execution tools.
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
	if (ms->exec_tokens && ms_is_builtin(ms->exec_tokens->content))
		ms_execute_builtin(ms);
	else if (ms->exec_tokens)
		ms_handle_system_command(ms, arr);
	ms_executor_cleanup(ms, arr, stdout_b, stdin_b);
}
