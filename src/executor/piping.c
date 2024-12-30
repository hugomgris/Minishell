/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 12:37:48 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains helper/Flow-control functions for piping setup:
	-ms_create_pipes
	-ms_setup_child_pipes
	-ms_wait_children
*/

#include "../../includes/minishell.h"

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

void	ms_setup_child_pipes(t_ms *ms, int cmd_index, int pipe_count)
{
	int	i;

	if (ms->heredoc_fd != -1 && cmd_index == 0)
	{
		if (dup2(ms->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc failed");
			exit(1);
		}
		close(ms->heredoc_fd);
		ms->heredoc_fd = -1;
	}
	else if (cmd_index > 0)
		dup2(ms->pipe_fds[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < pipe_count)
		dup2(ms->pipe_fds[cmd_index][1], STDOUT_FILENO);
	i = -1;
	while (++i < pipe_count)
	{
		close(ms->pipe_fds[i][0]);
		close(ms->pipe_fds[i][1]);
	}
}

void	ms_wait_children(int count)
{
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		waitpid(-1, &status, 0);
		i++;
	}
}
