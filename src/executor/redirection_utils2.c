/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 13:04:06 by hmunoz-g         ###   ########.fr       */
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
Helper function to check if input string contains redirection tokens.
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
