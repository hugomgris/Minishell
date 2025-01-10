/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 18:48:17 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Allocates memory for the pipes based on the number of pipes needed.
Each pipe gets its own file descriptors.
Memory is freed if allocation fails.
*/
void	ms_create_pipes(t_ms *ms, int ***pipe_fds, int pipe_count)
{
	int	i;

	*pipe_fds = malloc(sizeof(int *) * pipe_count);
	if (!*pipe_fds)
		return (ms_error_handler(ms, "Error: Mem alloc failed", 1));
	i = 0;
	while (i < pipe_count)
	{
		(*pipe_fds)[i] = malloc(sizeof(int) * 2);
		if (!(*pipe_fds)[i])
		{
			while (--i >= 0)
				free((*pipe_fds)[i]);
			free(*pipe_fds);
			return (ms_error_handler(ms, "Error: Mem alloc failed", 1));
		}
		pipe((*pipe_fds)[i]);
		i++;
	}
}

/*
Sets up the file descriptors for pipes in a child process.
Redirects the input and output to the appropriate pipes based on the cmd index.
Closes all pipe file descriptors after setup.
*/
void	ms_setup_child_pipes(t_ms *ms, int cmd_index, int pipe_count)
{
	int	i;

	if (cmd_index > 0)
	{
		dup2(ms->pipe_fds[cmd_index - 1][0], STDIN_FILENO);
	}
	if (cmd_index < pipe_count)
	{
		dup2(ms->pipe_fds[cmd_index][1], STDOUT_FILENO);
	}
	i = -1;
	while (++i < pipe_count)
	{
		if (ms->pipe_fds[i][0] != -1)
		{
			if (close(ms->pipe_fds[i][0]) == -1)
				ms_error_handler(ms, "Error: close failed", 0);
		}
		if (ms->pipe_fds[i][1] != -1)
		{
			if (close(ms->pipe_fds[i][1]) == -1)
				ms_error_handler(ms, "Error: close failed", 0);
		}
	}
}

/*
Waits for all child processes to finish execution.
Keeps track of the child processes and their exit statuses.
*/
int	ms_wait_children(t_ms *ms, int count)
{
	int		i;
	int		status;
	pid_t	pid;

	ms_get_set(SET, SHELL_CHILD_PROCESS);
	ms_cleanup_heredoc(ms);
	if (count <= 0)
		return (0);
	i = 0;
	while (i < count)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
		{
			if (errno != ECHILD)
				ms_error_handler(ms, "minishell: waitpid", 0);
			break ;
		}
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
		i++;
	}
	return (0);
}
