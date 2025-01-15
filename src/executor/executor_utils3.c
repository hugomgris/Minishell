/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/15 14:44:20 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Executes a single command in the Minishell.
Steps:
  1. Validates the cmd_args array for memory allocation.
  2. Handles redirection if required:
      - If redirection fails, exits for non-builtin commands.
  3. Checks if the command is a builtin:
      - Executes the builtin and handles rerouting if necessary.
  4. For non-builtin commands:
      - Delegates execution to ms_handle_system_cmd.
      - Cleans up memory if system command handling fails.
Returns:
  - 1 on success or handled error.
  - Exits the process on fatal errors during redirection or command execution.
*/
int	ms_exec_command(t_ms *ms, char **env)
{
	if (!ms->cmd_args)
		return (ms_error_handler(ms, "Error: Mem alloc failed", 1), 1);
	if (ms_has_redirection(ms))
	{
		if (ms_redirection(ms) == -1)
		{
			if (ms_is_builtin(ms->filt_args[0]))
				return (1);
			exit(1);
		}
	}
	if (ms_is_builtin(ms->cmd_args[0]))
	{
		if (ms_reroute_builtins(ms, env))
			return (1);
	}
	else if (ms_handle_system_cmd(ms, env) == -1)
	{
		ft_free(ms->cmd_args);
		ft_free(ms->filt_args);
		return (1);
	}
	return (0);
}

int	ms_has_input_redirection(t_ms *ms)
{
	int	i;

	i = -1;
	while (ms->cmd_args[++i])
	{
		if (!ft_strcmp(ms->cmd_args[i], "<"))
			return (1);
	}
	return (0);
}

/*
Handles the child process execution logic for a Minishell command.
Received int i is the current exec chunk index in the exec pipeline.
Steps:
  1. If a heredoc is present, duplicates its file descriptor to STDIN_FILENO.
  2. Sets up the appropriate pipes for the child process.
  3. Handles redirection unless a heredoc is present.
  4. Executes the command (builtin or system).
  5. Exits the child process with a failure status if any error occurs.
*/
int	ms_handle_child_process(t_ms *ms, char **env, int i)
{
	ms_setup_child_pipes(ms, i, ms->pipe_count);
	if (ms_has_redirection(ms))
	{
		if (ms_redirection(ms) == -1)
			exit(1);
	}
	if (i == 0 && ms->heredoc_fd != -1 && !ms_has_input_redirection(ms))
	{
		if (dup2(ms->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("heredoc dup2 failed");
			ms_error_handler(ms, "Heredoc dup2 failed", 0);
			exit(1);
		}
		close(ms->heredoc_fd);
	}
	else if (i > 0)
		ms->heredoc_fd = -1;
	if (ms_exec_command(ms, env) != 0)
		exit(1);
	exit(127);
}

/*
Handles the parent process logic for Minishell.
Steps:
  1. Closes the heredoc file descriptor if it was used.
  2. Resets the heredoc_fd in the Minishell state to indicate closure.
*/
int	ms_handle_parent_process(t_ms *ms)
{
	if (ms->heredoc_fd != -1)
	{
		close(ms->heredoc_fd);
		ms->heredoc_fd = -1;
	}
	return (0);
}

/*
Closes the appropriate pipe file descriptors after use.
Steps:
  1. Closes the read end of the previous pipe (if applicable).
  2. Closes the write end of the current pipe.
*/
void	ms_close_used_pipes(int **pipe_fds, int i)
{
	if (i > 0)
		close(pipe_fds[i - 1][0]);
	if (pipe_fds[i])
		close(pipe_fds[i][1]);
}
