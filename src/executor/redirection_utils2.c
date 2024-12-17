/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/17 11:16:53 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_open_tmp_heredoc(void)
{
	int	fd;

	fd = open("/tmp/heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	return (fd);
}

int	ms_handle_heredoc_signal(int tmp_fd, int *fd)
{
	close(tmp_fd);
	unlink("/tmp/heredoc_tmp");
	ms_get_set(1, 0);
	*fd = -1;
	return (-1);
}

int	ms_write_heredoc_lines(int tmp_fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL || ms_get_set(0, 0) == 3)
			return (-1);
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter)))
			break ;
		ft_putstr_fd(line, tmp_fd);
		ft_putchar_fd('\n', tmp_fd);
		free(line);
	}
	free(line);
	return (0);
}

int	ms_finalize_heredoc(int tmp_fd, int *fd)
{
	close(tmp_fd);
	*fd = open("/tmp/heredoc_tmp", O_RDONLY);
	if (*fd == -1)
		return (-1);
	unlink("/tmp/heredoc_tmp");
	return (0);
}

int	ms_handle_heredoc(char *delimiter, int *fd)
{
	int	tmp_fd;

	ms_get_set(1, 2);
	tmp_fd = ms_open_tmp_heredoc();
	if (tmp_fd == -1)
		return (-1);
	if (ms_write_heredoc_lines(tmp_fd, delimiter) == -1)
		return (ms_handle_heredoc_signal(tmp_fd, fd));
	return (ms_finalize_heredoc(tmp_fd, fd));
}
