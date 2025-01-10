/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils7.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 18:13:13 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Cleans up the executor env by clearing lists and freeing allocated memory.
Steps:
  1. Clears the list of `chain_tokens`, freeing each element.
  2. Frees the `exec_chunks` array and the environment array.
*/
void	ms_executor_cleanup(t_ms *ms, char	**env)
{
	ft_free(ms->exec_chunks);
	ft_free(env);
}

/*
Cleans up arguments by freeing allocated memory.
Steps:
  1. Checks if `filt_args` is not NULL, then frees it.
  2. Checks if `cmd_args` is not NULL, then frees it.
*/
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

/*
Cleans up heredoc file descriptor.
Steps:
  1. Checks if the heredoc file descriptor (heredoc_fd) is valid (not -1).
  2. If valid, closes the file descriptor and resets it to -1.
*/
void	ms_cleanup_heredoc(t_ms *ms)
{
	if (ms->heredoc_fd != -1)
	{
		if (close(ms->heredoc_fd) == -1)
		{
			ms_error_handler(ms, "Error: close failed", 0);
			return ;
		}
		ms->heredoc_fd = -1;
	}
}

/*
Saves the current standard input, output, and error file descriptors.
Steps:
  1. Duplicates the current standard file descriptors.
  2. Saves STD FDS in the saved_fds array.
*/
void	ms_save_std_fds(int *saved_fds)
{
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	saved_fds[2] = dup(STDERR_FILENO);
}

/*
Restores the previously saved standard input, output, and error file descriptors.
Steps:
  1. Restores the file descriptors from the saved_fds array using dup2().
  2. Closes the saved file descriptors.
*/
void	ms_restore_std_fds(t_ms *ms, int *saved_fds)
{
	dup2(saved_fds[0], STDIN_FILENO);
	dup2(saved_fds[1], STDOUT_FILENO);
	dup2(saved_fds[2], STDERR_FILENO);
	if (close(saved_fds[0]) == -1 || close(saved_fds[1]) == -1
		|| close(saved_fds[2]) == -1)
		ms_error_handler(ms, "Error: close failed", 0);
}
