/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/20 18:24:41 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains helper/Flow-control functions for piping closing and cleanup:
	-ms_free_pipes
	-ms_close_child_pipes
	-ms_close_parent_pipes
*/

#include "../../includes/minishell.h"

void	ms_free_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		free(pipe_fds[i]);
		i++;
	}
	free(pipe_fds);
}

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

void	ms_close_parent_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		i++;
	}
}
