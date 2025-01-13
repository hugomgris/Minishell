/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 20:48:15 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Frees the allocated memory for pipe file descriptors.
It iterates through each pipe and frees the allocated memory for both ends,
	and then frees the memory holding the array of pipe file descriptors.
*/
void	ms_free_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

	i = -1;
	while (++i < pipe_count)
		free(pipe_fds[i]);
	free(pipe_fds);
}

/*
Closes all the pipe file descriptors for child processes.
This is done after the pipes have been used for redirection
	to prevent resource leakage.
*/
void	ms_close_child_pipes(int **pipe_fds, int pipe_count)
{
	int	j;

	j = 0;
	while (j < pipe_count)
	{
		close(pipe_fds[j][0]);
		close(pipe_fds[j][1]);
		j++;
	}
}

/*
Closes all the pipe file descriptors for the parent process.
After the pipes are no longer needed, this function ensures that
	the pipe file descriptors are closed.
*/
void	ms_close_parent_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

	i = -1;
	while (++i < pipe_count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
	}
}
