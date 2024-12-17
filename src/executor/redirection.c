/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/17 11:55:25 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_open(char *file, int flags, int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = open(file, flags, 0644);
	if (*fd == -1)
		return (-1);
	return (0);
}

int	ms_setup_redirects(t_list *token, int *fds, t_ms *ms)
{
	int	type;

	type = ms_detect_redirector(token->content);
	if (type && (!token->next || ms_detect_redirector(token->next->content)))
		return (ms_error_handler(ms, "Invalid redirection", 0), -1);
	if ((type == 1 && ms_open(token->next->content, O_RDONLY, &fds[0]))
		|| (type == 2 && ms_open(token->next->content, O_WRONLY
				| O_CREAT | O_TRUNC, &fds[1]))
		|| (type == 3 && ms_open(token->next->content, O_WRONLY
				| O_CREAT | O_APPEND, &fds[1]))
		|| (type == 4 && ms_open(token->next->content, O_WRONLY
				| O_CREAT | O_TRUNC, &fds[2])))
		return (ms_handle_open_error(ms, token->next->content), 0);
	if (type == 5)
	{
		if (ms_handle_heredoc(token->next->content, &fds[0]) == -1)
		{
			if (fds[0] == -1)
				return (-1);
			return (ms_error_handler(ms, "Heredoc error", 0), -1);
		}
	}
	return (0);
}

int	ms_redirection(t_ms *ms)
{
	t_list	*token;
	int		fds[3];

	fds[0] = -1;
	fds[1] = -1;
	fds[2] = -1;
	token = ms->tokens;
	while (token)
	{
		if (ms_setup_redirects(token, fds, ms) == -1)
			return (-1);
		token = token->next;
	}
	if (fds[0] != -1 && dup2(fds[0], STDIN_FILENO) == -1)
		return (-1);
	if (fds[1] != -1 && dup2(fds[1], STDOUT_FILENO) == -1)
		return (-1);
	if (fds[2] != -1 && dup2(fds[2], STDERR_FILENO) == -1)
		return (-1);
	ms_close_redirect_fds(fds[0], fds[1], fds[2], -1);
	return (0);
}
