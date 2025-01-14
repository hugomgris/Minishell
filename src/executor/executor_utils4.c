/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 20:59:43 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Checks if the current command includes a heredoc ('<<').
Steps:
  1. Iterates through the cmd_args array to find the '<<' token.
  2. Returns 1 if a heredoc is present, 0 otherwise.
TODO: check return values for consistency, may need to be switched.
*/
int	ms_has_heredoc(t_ms *ms)
{
	int	i;
	int	hd;

	i = -1;
	hd = 0;
	while (ms->cmd_args[++i])
	{
		if (!ft_strncmp(ms->cmd_args[i], "<<", 2))
			hd++;
	}
	return (hd);
}

/*
Processes the heredoc setup for a command.
Steps:
  1. Searches for the '<<' token in cmd_args to identify the delimiter.
  2. If no delimiter is found, handles the error and returns -1.
  3. Calls ms_handle_heredoc to process the heredoc and obtain the FD.
  4. Updates heredoc_fd in the Minishell state if successful.
  5. Returns 0 on success, -1 on failure.
*/
int	ms_process_heredoc(t_ms *ms)
{
	int		fd;
	char	*delimiter;
	int		i;
	int		result;

	delimiter = NULL;
	i = -1;
	while (ms->cmd_args[++i])
	{
		if (!ft_strncmp(ms->cmd_args[i], "<<", 2))
		{
			delimiter = ms->cmd_args[i + 1];
		}
	}
	if (!delimiter)
		return (ms_handle_heredoc_error(ms, "Heredoc delimiter missing"));
	result = ms_handle_heredoc(delimiter, &fd);
	if (result == -1)
		return (ms_handle_heredoc_error(ms, "Heredoc processing failed"));
	ms->heredoc_fd = fd;
	return (0);
}

/*
Sets up the environment for heredoc processing.
Steps:
  1. Saves the current stdin and stdout file descriptors.
  2. Redirects stdin and stdout to /dev/tty for interactive input.
  3. Calls ms_process_heredoc to handle the heredoc processing.
  4. Restores the original stdin and stdout file descriptors.
  5. Cleans up resources and returns the result of heredoc processing.
  6. Returns 0 on success, -1 on failure.
*/
int	ms_handle_heredoc_setup(t_ms *ms)
{
	int	orig_stdin;
	int	orig_stdout;
	int	result;
	int	tty;

	orig_stdin = dup(STDIN_FILENO);
	orig_stdout = dup(STDOUT_FILENO);
	tty = open("/dev/tty", O_RDWR);
	if (tty != -1)
	{
		dup2(tty, STDIN_FILENO);
		dup2(tty, STDOUT_FILENO);
		close(tty);
	}
	result = ms_process_heredoc(ms);
	dup2(orig_stdin, STDIN_FILENO);
	dup2(orig_stdout, STDOUT_FILENO);
	close(orig_stdin);
	close(orig_stdout);
	return (result);
}

/*
Handles errors encountered during heredoc setup or processing.
Steps:
  1. Checks the current shell state using ms_get_set.
  2. Logs the provided error message if the shell state is 2 (heredoc input).
  3. Returns -1 to indicate failure.
*/
int	ms_handle_heredoc_error(t_ms *ms, char *error_msg)
{
	if (ms_get_set(GET, 0) == 2)
		ms_error_handler(ms, error_msg, 0);
	return (-1);
}
