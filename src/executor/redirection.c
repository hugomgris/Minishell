/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 13:01:59 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to open files and assign FDs to them.
*/
int	ms_open(char *file, int flags, int *fd)
{
	if (*fd != -1)
		close (*fd);
	*fd = open(file, flags, 0644);
	if (*fd == -1)
		return (-1);
	return (0);
}

int	ms_redirect_fd(int old_fd, int new_fd, t_ms *ms, int code)
{
	char	*msg;

	if (old_fd != -1 && dup2(old_fd, new_fd) == -1)
	{
		if (code == 1)
		{
			msg = "Error: Failed to redir input";
			return (ms_error_handler(ms, msg, 0), -1);
		}
		else if (code == 2)
		{
			msg = "Error: Failed to redir output";
			return (ms_error_handler(ms, msg, 0), -1);
		}
		else if (code == 3)
		{
			msg = "Error: Failed to redirect error";
			return (ms_error_handler(ms, msg, 0), -1);
		}
	}
	return (0);
}

int	ms_redirection(t_ms *ms)
{
	int	i;
	int	fds[3];

	fds[0] = -1;
	fds[1] = -1;
	fds[2] = -1;
	i = -1;
	while (ms->cmd_args[++i])
		if (ms_detect_redirector(ms->cmd_args[i])
			&& ms_setup_redirects(ms->cmd_args, i++, fds, ms) == -1)
			return (ms_close_redirect_fds(fds[0], fds[1], fds[2], -1), -1);
	if (ms_manage_heredoc(ms, fds) == -1)
		return (-1);
	if (ms_redirect_fd(fds[0], STDIN_FILENO, ms, 1) == -1
		|| ms_redirect_fd(fds[1], STDOUT_FILENO, ms, 2) == -1
		|| ms_redirect_fd(fds[2], STDERR_FILENO, ms, 3) == -1)
		return (-1);
	return (ms_close_redirect_fds(fds[0], fds[1], fds[2], -1));
}
