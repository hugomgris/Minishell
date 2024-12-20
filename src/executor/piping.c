/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/20 18:25:18 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains helper/Flow-control functions for piping setup:
	-ms_create_pipes
	-ms_setup_child_pipes
	-ms_wait_children
*/

#include "../../includes/minishell.h"

void	ms_create_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		pipe_fds[i] = malloc(sizeof(int) * 2);
		pipe(pipe_fds[i]);
		i++;
	}
}

void	ms_setup_child_pipes(int **pipe_fds, int i, int pipe_count)
{
	if (i > 0)
		dup2(pipe_fds[i - 1][0], STDIN_FILENO);
	if (i < pipe_count)
		dup2(pipe_fds[i][1], STDOUT_FILENO);
}

void	ms_wait_children(int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		wait(NULL);
		i++;
	}
}
