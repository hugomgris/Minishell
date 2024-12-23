/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/23 11:21:36 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_filter_args(t_ms *ms)
{
	int	i;
	int	count;
	int	f_pos;

	count = 0;
	i = -1;
	while (ms->cmd_args[++i])
	{
		if (!ms_detect_redirector(ms->cmd_args[i]))
			count++;
	}
	ms->filt_args = malloc(sizeof(char *) * (count + 1));
	if (!ms->filt_args)
	{
		ms_error_handler(ms, "Error: Mem alloc failed", 1);
		return ;
	}
	i = 0;
	f_pos = 0;
	while (ms->cmd_args[i])
	{
		if (ms_detect_redirector(ms->cmd_args[i]))
		{
			i += 2;
			if (i >= count)
				break ;
		}
		if (!ms_detect_redirector(ms->cmd_args[i]))
		{
			ms->filt_args[f_pos] = ms->cmd_args[i];
			i++;
			f_pos++;
		}
	}
	ms->filt_args[f_pos] = NULL;
}

int	ms_handle_open_error(t_ms *ms, char *filename)
{
	(void)filename;
	ms_error_handler(ms, "Error: No suh file or directory", 0);
	return (-1);
}

void	ms_close_redirect_fds(int input, int output, int append, int stderr_fd)
{
	if (input != -1)
		close(input);
	if (output != -1)
		close(output);
	if (append != -1)
		close(append);
	if (stderr_fd != -1)
		close(stderr_fd);
}
