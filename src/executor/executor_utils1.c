/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/18 09:20:30 by hmunoz-g         ###   ########.fr       */
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
	if (!cmd_path)
		ms_error_handler(ms, "Command not found or execve failed", 0);
	if (execve(cmd_path, ms_make_argv(ms, ms->exec_tokens), arr) == -1)
		ms_error_handler(ms, "Command execution failed", 0);
	gc_add(cmd_path, &ms->gc);
}

/*
Makes the parent process wait until child process ends.
*/
void	ms_parent_process(t_ms *ms, pid_t pid, int *status)
{
	if (waitpid(pid, status, 0) == -1)
	{
		ms_error_handler(ms, "Error: Waitpid failed", 0);
		ms_get_set(1, 1);
		return ;
	}
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
void	execute_parent(t_ms *ms, pid_t pid, int *status)
{
	ms_get_set(1, 1);
	ms_parent_process(ms, pid, status);
}
