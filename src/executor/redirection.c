/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/08 16:31:49 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_check_file_statuses(char **args)
{
	int		i;
	char	*filename;

	i = -1;
	while (args[++i])
	{
		if (!ft_strncmp(args[i], "<", 1))
		{
			filename = args[i + 1];
			if (access(filename, F_OK) == -1)
				return (i + 1);
			if (access(filename, R_OK) == -1)
				return (i + 1);
		}
	}
	return (0);
}

/*
Helper function for input redirection (<).
Gets the latest value (file) for the redirection in the input.
*/
int	ms_latest_infile(char **args)
{
	int	count;
	int	status;

	status = ms_check_file_statuses(args);
	if (status)
		return (status);
	count = ft_array_count(args);
	while (args[--count] > 0)
	{
		if (!ft_strncmp(args[count], "<", 1))
			return (count + 1);
	}
	return (1);
}

/*
Opens a file with the specified flags and assigns its FD to the provided var.
If an FD is already open, it is closed before opening the new file. 
Returns 0 on success or -1 if the file could not be opened.
TODO: Again, check return values for consistency (0-1 vs 0-(-1))
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

/*
Redirects a given file descriptor to a new one using dup2. 
Handles different types of redirections by checking the code value
	and reports errors via ms_error_handler. 
Returns 0 on success or -1 if redirection fails.
TODO: Again, check return values for consistency (0-1 vs 0-(-1))
*/
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

/*
Handles the setup and management of file redirections based on command args. 
Also handles heredoc management and ensures that the appropriate redirections
	are applied to standard file descriptors. 
If any step fails, it returns -1. Otherwise, it returns 0.
TODO: Again, check return values for consistency (0-1 vs 0-(-1))
*/
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
	if (!ms->pipe_count || (ms->pipe_count && fds[0] != -1))
	{
		if (ms_redirect_fd(fds[0], STDIN_FILENO, ms, 1) == -1)
			return (-1);
	}
	if (ms_redirect_fd(fds[1], STDOUT_FILENO, ms, 2) == -1
		|| ms_redirect_fd(fds[2], STDERR_FILENO, ms, 3) == -1)
		return (-1);
	return (ms_close_redirect_fds(fds[0], fds[1], fds[2], -1));
}
