/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/18 12:09:11 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to filter non-redirection tokens from input string.
*/
void	ms_filter_tokens(t_ms *ms)
{
	t_list	*current;
	char	*token;
	char	*n_token;

	current = ms->tokens;
	while (current)
	{
		token = current->content;
		if (current->next)
			n_token = current->next->content;
		if (ms_detect_redirector(token))
		{
			ft_lstadd_back(&ms->redir_tokens, ft_lstnew(token));
			ft_lstadd_back(&ms->redir_tokens, ft_lstnew(n_token));
			current = current->next;
		}
		else if (!ft_strncmp(token, "|", 1) && !token[1])
			ft_lstadd_back(&ms->pipe_tokens, ft_lstnew(token));
		else
			ft_lstadd_back(&ms->exec_tokens, ft_lstnew(token));
		current = current->next;
	}
	ft_printf("------tokens:\n");
	ms_print_list(ms->tokens);
	ft_printf("------redir_tokens:\n");
	ms_print_list(ms->redir_tokens);
	ft_printf("------pipe_tokens:\n");
	ms_print_list(ms->pipe_tokens);
	ft_printf("------exec_tokens:\n");
	ms_print_list(ms->exec_tokens);
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
	t_list	*tokens;

	tokens = ms->redir_tokens;
	while (tokens)
	{
		if (ms_detect_redirector(tokens->content))
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

/*
Helper function to send specific redirection file errors to error handler.
Used in case an input redirection file doesn't exist. 
*/
int	ms_handle_open_error(t_ms *ms, char *filename)
{
	char	*output;

	output = ft_strjoin(filename, ": No such file or directory");
	gc_add(output, &ms->gc);
	ms_error_handler(ms, output, 0);
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
