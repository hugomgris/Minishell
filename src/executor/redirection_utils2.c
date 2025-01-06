/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/04 13:15:57 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to detect redirection types in the input tokens.
Detects cases:
	-Output append (>>)
	-Output rewrite (>)
	-Error output redirection (2>)
	-Input file (<)
	-Heredoc (<<)
*/
int	ms_detect_redirector(char *arg)
{
	if (ft_strncmp(arg, ">>", 2) == 0)
		return (3);
	else if (ft_strncmp(arg, "2>", 2) == 0)
		return (4);
	else if (ft_strncmp(arg, "<<", 2) == 0)
		return (5);
	else if (ft_strncmp(arg, ">", 1) == 0)
		return (2);
	else if (ft_strncmp(arg, "<", 1) == 0 && !arg[1])
		return (1);
	return (0);
}

/*
Checks if there are any redirection operators in the given command arguments.
Returns 1 if a redirection operator is found, otherwise 0.
TODO: Again, check return values for consistency (might need to swap 1-0)
*/
int	ms_has_redirection(t_ms *ms)
{
	int	i;

	i = -1;
	while (ms->cmd_args[++i])
	{
		if (ms_detect_redirector(ms->cmd_args[i]))
			return (1);
	}
	return (0);
}

/*
Sets up the file descriptors for input/output redirection.
Checks for valid syntax and handles the redirection by opening
	the appropriate files.
If the redirection syntax is invalid or there is an error opening a file,
	it returns an error message.
TODO: this could use a more granular error handling.
*/
int	ms_setup_redirects(char **args, int i, int *fds, t_ms *ms)
{
	int	type;

	type = ms_detect_redirector(args[i]);
	if (type && (!args[i + 1] || ms_detect_redirector(args[i + 1])))
		return (ms_error_handler(ms, "Error: Invalid redir syntax", 0), -1);
	if (fds[0] == -1)
	{
		if (type == 1 && ms_open(args[i + 1], O_RDONLY, &fds[0]))
			return (ms_handle_open_error(ms, args[i + 1]));
	}
	if (type == 2 && ms_open(args[i + 1], O_WRONLY
			| O_CREAT | O_TRUNC, &fds[1]))
		return (ms_handle_open_error(ms, args[i + 1]));
	if (type == 3 && ms_open(args[i + 1], O_WRONLY
			| O_CREAT | O_APPEND, &fds[1]))
		return (ms_handle_open_error(ms, args[i + 1]));
	if (type == 4 && ms_open(args[i + 1], O_WRONLY
			| O_CREAT | O_TRUNC, &fds[2]))
		return (ms_handle_open_error(ms, args[i + 1]));
	return (0);
}

/*
Manages the file descriptors for heredoc input redirection.
If there is an open heredoc file descriptor, it updates
	the input file descriptor array to use it.
This is used for managing multiple redirections in a command chain.
*/
int	ms_manage_heredoc(t_ms *ms, int *fds)
{
	if (ms->heredoc_fd != -1)
	{
		if (fds[0] != -1)
			close(fds[0]);
		fds[0] = ms->heredoc_fd;
	}
	return (0);
}

/*
Closes the given file descriptors for input, output, append, and error streams.
This is used to clean up file descriptors after redirections are completed.
*/
int	ms_close_redirect_fds(int input, int output, int append, int stderr_fd)
{
	if (input != -1)
		close(input);
	if (output != -1)
		close(output);
	if (append != -1)
		close(append);
	if (stderr_fd != -1)
		close(stderr_fd);
	return (0);
}
