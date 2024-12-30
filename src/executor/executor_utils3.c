/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 13:13:05 by hmunoz-g         ###   ########.fr       */
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

	i = -1;
	while (ms->cmd_args[++i])
	{
		if (!ft_strncmp(ms->cmd_args[i], "<<", 2))
			delimiter = ms->cmd_args[i + 1];
	}
	if (ms_handle_heredoc(delimiter, &fd) == -1)
		return (ms_error_handler(ms, "Heredoc processing failed", 0), -1);
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
