/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/12 20:15:25 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Searches for commands using the env path.
If command is not found, returns an error.
*/
void	ms_child_process(t_ms *ms, char **arr)
{
	char	*cmd_path;

	cmd_path = ms_get_command_path(ms, ms->tokens->content);
	if (!cmd_path || execve(cmd_path, ms_make_argv(ms, ms->tokens), arr) == -1)
		ms_error_handler(ms, "Command not found or execve failed", 0);
	gc_add(cmd_path, &ms->gc);
}

/*
Makes the parent process wait until child process ends.
*/
void	ms_parent_process(pid_t pid, int *status)
{
	waitpid(pid, status, 0);
	ms_get_set(1, WEXITSTATUS(*status));
}

/*
Control flow function that calls for child process execution.
Rises the flag for the SIGINT handler to know if its in child or parent proc.
*/
void	execute_child(t_ms *ms, char **arr)
{
	ms_get_set(1, 0);
	ms_child_process(ms, arr);
}

/*
Control flow function that calls for parent process execution.
Rises the flag for the SIGINT handler to know if its in child or parent proc.
*/
void	execute_parent(pid_t pid, int *status)
{
	ms_get_set(1, 1);
	ms_parent_process(pid, status);
}

/*
Executor hub.
Makes some checks to know if it needs to execute a builtin or a system cmd.
Creates forks for child processes, calls for execution of both child and parent.
Calls the executor cleanup.
*/
void	ms_executor(t_ms *ms)
{
	pid_t	pid;
	int		status;
	char	**arr;
	char	*path;

	arr = ms_env_to_array(ms->ms_env);
	path = validate_command(ms);
	if (ms_is_builtin(ms->tokens->content))
		ms_execute_builtin(ms);
	else if (path)
	{
		gc_add(path, &ms->gc);
		pid = fork();
		if (pid == -1)
			ms_error_handler(ms, "Fork failed", 0);
		else if (pid == 0)
			execute_child(ms, arr);
		else
			execute_parent(pid, &status);
	}
	ms_executor_cleanup(ms, arr);
}
