/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils9.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/16 09:26:25 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to handle space arguments (e.g.: tr " " "x").
It's the first of a two step process, in which spaces get 
	temporarily changed to " so that tokenization don't skip them. 
*/
char	*ms_process_space_args_in(char *chunk)
{
	unsigned long	i;

	i = 1;
	while (chunk[i] && i < ft_strlen(chunk) - 2)
	{
		if (ft_isspace(chunk[i - 1]) && ft_isspace(chunk[i])
			&& ft_isspace(chunk[i + 1]))
			chunk[i] = '"';
		i++;
	}
	return (chunk);
}

/*
Helper function to handle space arguments (e.g.: tr " " "x").
It's the second of a two step process, in which temporarily
	changed tokens (") get changed back to a space char.
*/
char	**ms_process_space_args_out(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if (args[i][0] == '"')
			args[i][0] = ' ';
	}
	return (args);
}

/*
Handles the interruption of a heredoc operation
	(e.g., when the user presses Ctrl+C).
- Resets the shell state to normal using `ms_get_set`.
- Cleans up execution resources with `ms_executor_cleanup`.
- Frees pipe resources with `ms_free_pipes`.
- Cleans up heredoc-related resources with `ms_cleanup_heredoc`.
- Sets the exit status to 130 (indicating interruption by signal).
Returns 130 to propagate the interruption status.
*/
int	ms_heredoc_interruption(t_ms *ms, char **env)
{
	ms_get_set(SET, SHELL_NORMAL);
	ms_executor_cleanup(ms, env);
	ms_free_pipes(ms->pipe_fds, ms->pipe_count);
	ms_cleanup_heredoc(ms);
	ms->exit_status = 130;
	return (130);
}

/*
Retrieves the separator used in a heredoc operation from the token chain.
- Iterates through the token chain (`ms->chain_tokens`).
- Identifies the heredoc separator (the token following `<<`).
- Duplicates the separator and adds it to the garbage collector for cleanup.
Returns the separator string.
*/
char	*ms_get_separator(t_ms *ms)
{
	t_token	*curr;
	char	*sep;

	curr = ms->chain_tokens;
	while (curr)
	{
		if (ft_strcmp(curr->content, "<<"))
		{
			sep = ft_strdup(curr->content);
			gc_add(sep, &ms->gc);
		}
		curr = curr->next;
	}
	return (sep);
}
