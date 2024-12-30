/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 12:57:12 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains helper/Flow-control functions for exec tools cleanup:
	-ms_executor_cleanup
	-ms_cleanup_args
	-ms_save_std_fds
	-ms_restore_std_fds
*/

#include "../../includes/minishell.h"

void	ms_executor_cleanup(t_ms *ms, char	**env)
{
	ft_lstclear(&ms->tokens, free);
	ft_free(ms->exec_chunks);
	ft_free(env);
}

void	ms_cleanup_args(t_ms *ms)
{
	if (ms->filt_args[0] != NULL)
	{
		free(ms->filt_args);
		ms->filt_args = NULL;
	}
	if (ms->cmd_args[0] != NULL)
	{
		free(ms->cmd_args);
		ms->cmd_args = NULL;
	}
}

void	ms_cleanup_heredoc(t_ms *ms)
{
	if (ms->heredoc_fd != -1)
	{
		close(ms->heredoc_fd);
		ms->heredoc_fd = -1;
	}
}

void	ms_save_std_fds(int *saved_fds)
{
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	saved_fds[2] = dup(STDERR_FILENO);
}

void	ms_restore_std_fds(int *saved_fds)
{
	dup2(saved_fds[0], STDIN_FILENO);
	dup2(saved_fds[1], STDOUT_FILENO);
	dup2(saved_fds[2], STDERR_FILENO);
	close(saved_fds[0]);
	close(saved_fds[1]);
	close(saved_fds[2]);
}
