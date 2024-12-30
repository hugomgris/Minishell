/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 15:36:21 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains helper/Flow-control functions to build exec components:
	-ms_env_to_aray
	-ms_rebuild_env
*/

#include "../../includes/minishell.h"

int	ms_has_heredoc(t_ms *ms)
{
	int	i;

	i = -1;
	while (ms->cmd_args[++i])
	{
		if (!ft_strncmp(ms->cmd_args[i], "<<", 2))
			return (1);
	}
	return (0);
}

int	ms_process_heredoc(t_ms *ms)
{
	int		fd;
	char	*delimiter;
	int		i;
	int		result;

	i = -1;
	delimiter = NULL;
	while (ms->cmd_args[++i])
		if (!ft_strncmp(ms->cmd_args[i], "<<", 2))
			delimiter = ms->cmd_args[i + 1];
	if (!delimiter)
		return (ms_handle_heredoc_error(ms, "Heredoc delimiter missing"));
	result = ms_handle_heredoc(delimiter, &fd);
	if (result == -1)
		return (ms_handle_heredoc_error(ms, "Heredoc processing failed"));
	ms->heredoc_fd = fd;
	return (0);
}

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

int	ms_handle_heredoc_error(t_ms *ms, char *error_msg)
{
	if (ms_get_set(0, 0) == 2)
		ms_error_handler(ms, error_msg, 0);
	return (-1);
}
