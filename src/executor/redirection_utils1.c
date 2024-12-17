/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/17 15:37:39 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to filter non-redirection tokens from input string.
*/
t_list	*ms_filter_tokens(t_list *tokens)
{
	t_list	*filtered;
	t_list	*current;

	filtered = NULL;
	current = tokens;
	while (current)
	{
		if (!ms_detect_redirector(current->content))
			ft_lstadd_back(&filtered, ft_lstnew(ft_strdup(current->content)));
		else
			current = current->next;
		current = current->next;
	}
	return (filtered);
}

/*
Helper function to detect redirection types in the input tokens.
Detects cases:
	-Output append (>>)
	-Output rewrite (>)
	-Error output redirection (2>)
	-Input file (<)
	-Heredoc (<<)
*/
int	ms_detect_redirector(char *token)
{
	if (ft_strncmp(token, ">>", 2) == 0)
		return (3);
	else if (ft_strncmp(token, "2>", 2) == 0)
		return (4);
	else if (ft_strncmp(token, "<<", 2) == 0)
		return (5);
	else if (ft_strchr(token, '>'))
		return (2);
	else if (ft_strchr(token, '<'))
		return (1);
	return (0);
}

/*
Helper function to check if input string contains redirection tokens.
*/
int	ms_has_redirection(t_ms *ms)
{
	t_list	*token;

	token = ms->tokens;
	while (token)
	{
		if (ms_detect_redirector(token->content))
			return (1);
		token = token->next;
	}
	return (0);
}

/*
Helper function to send specific redirection file errors to error handler.
Used in case an input redirection file doesn't exist. 
*/
int	ms_handle_open_error(t_ms *ms, char *filename)
{
	(void)filename;
	ms_error_handler(ms, "No such file or directory", 0);
	return (-1);
}

/*
Helper function to close redirection FDs.
Handles 4 possible FD cases:
	-Input file (<)
	-Output file (>)
	-Append file (>>)
	-STDERR redirection (2>)
*/
void	ms_close_redirect_fds(int input, int output, int append, int stderr_fd)
{
	if (input != -1)
		close(input);
	if (output != -1)
		close(output);
	if (append != -1)
		close(append);
	if (stderr_fd != -1)
		close(stderr_fd);
}
