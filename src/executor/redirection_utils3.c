/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/31 12:37:35 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Creates a temporary file for the heredoc feature.
Returns the file descriptor for the temporary file.
*/
int	ms_open_tmp_heredoc(void)
{
	int	fd;

	fd = open("/tmp/heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	return (fd);
}

/*
Handles the cleanup process for a heredoc signal.
Closes the temporary heredoc file, removes it from the filesystem,
	and updates the heredoc state.
*/
int	ms_handle_heredoc_signal(int tmp_fd, int *fd)
{
	close(tmp_fd);
	unlink("/tmp/heredoc_tmp");
	ms_get_set(1, 0);
	*fd = -1;
	return (-1);
}

/*
Writes the lines of a heredoc to the temporary file.
Prompts the user for input until the delimiter is entered,
	then writes each line to the file.
The writing process ends when the delimiter is matched or an error occurs.
*/
int	ms_write_heredoc_lines(int tmp_fd, const char *delimiter)
{
	char	*line;

	while (42)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			if (ms_get_set(0, 0) == 3)
				return (-1);
			close(tmp_fd);
			return (0);
		}
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter))
			&& !line[ft_strlen(delimiter)])
			break ;
		ft_putstr_fd(line, tmp_fd);
		ft_putchar_fd('\n', tmp_fd);
		free(line);
	}
	free(line);
	return (0);
}

/*
Finalizes the heredoc process by closing the temporary file
	and reopening it for reading.
The file is removed after it is reopened to clean up.
*/
int	ms_finalize_heredoc(int tmp_fd, int *fd)
{
	close(tmp_fd);
	*fd = open("/tmp/heredoc_tmp", O_RDONLY);
	if (*fd == -1)
		return (-1);
	unlink("/tmp/heredoc_tmp");
	return (0);
}

/*
Handles the full heredoc process: creating the temporary file,
	writing the heredoc lines,
and setting up the file descriptor for reading the heredoc content.
If an error occurs, it handles the cleanup and returns an error.
*/
int	ms_handle_heredoc(const char *delimiter, int *fd)
{
	int		tmp_fd;
	char	*tmp_file;

	ms_get_set(1, 2);
	tmp_file = "/tmp/.minishell_heredoc";
	tmp_fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd == -1)
		return (-1);
	if (ms_write_heredoc_lines(tmp_fd, delimiter) == -1)
		return (ms_handle_heredoc_signal(tmp_fd, fd));
	close(tmp_fd);
	*fd = open(tmp_file, O_RDONLY);
	unlink(tmp_file);
	ms_get_set(1, 0);
	if (*fd == -1)
		return (-1);
	return (0);
}
