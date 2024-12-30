/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 13:16:48 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains 
*/

#include "../../includes/minishell.h"

// Main execution function
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
	return (1);
}

/*
Executor hub.
*/
void	ms_handle_child_process(t_ms *ms, char **env, int i)
{
	if (ms->heredoc_fd != -1 && dup2(ms->heredoc_fd, STDIN_FILENO) == -1)
	{
		ms_error_handler(ms, "Heredoc dup2 failed", 0);
		exit(1);
	}
	ms_setup_child_pipes(ms, i, ms->pipe_count);
	if (ms_has_redirection(ms) && !ms_has_heredoc(ms)
		&& ms_redirection(ms) == -1)
		exit(1);
	if (ms_exec_command(ms, env) != 0)
		exit(1);
	exit(0);
}

void	ms_handle_parent_process(t_ms *ms)
{
	if (ms->heredoc_fd != -1)
	{
		close(ms->heredoc_fd);
		ms->heredoc_fd = -1;
	}
}

void	ms_close_used_pipes(int **pipe_fds, int i)
{
	if (i > 0)
		close(pipe_fds[i - 1][0]);
	if (pipe_fds[i])
		close(pipe_fds[i][1]);
}
