/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/14 08:41:31 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Counts the number of execution chunks in the input tokens.
Chunks are divided by pipe tokens ('|').
Starts with a count of 1 and increments for each pipe encountered.
Returns the total number of execution chunks.
*/
int	ms_count_chunks(t_ms *ms, t_token *tokens)
{
	int		count;
	int		index;
	t_token	*current;

	(void)ms;
	count = 1;
	index = 0;
	current = tokens;
	while (current)
	{
		if (current->type == T_PIPE)
			count++;
		index++;
		current = current->next;
	}
	return (count);
}

/*
Processes a single chunk of tokens, concatenating its content into a string.
Stops processing at a pipe token ('|') or the end of the list.
Handles memory allocation errors and ensures safe cleanup in case of failure.
Moves the list pointer to the next chunk after processing.
Returns:
  - A string representing the chunk on success.
  - NULL on memory allocation failure.
*/
char	*ms_process_chunk(t_ms *ms, t_token **current)
{
	char	*chunk;
	char	*tmp;

	chunk = NULL;
	while (*current && (*current)->type != T_PIPE)
	{
		tmp = chunk;
		if (chunk)
			chunk = ft_strjoin3(chunk, " ", (*current)->content);
		else
			chunk = ft_strjoin_free(chunk, (*current)->content);
		if (!chunk)
		{
			ms_error_handler(ms, "Error: Mem alloc failed", 1);
			free(tmp);
			return (NULL);
		}
		free(tmp);
		*current = (*current)->next;
	}
	if (*current && (*current)->type == T_PIPE)
		*current = (*current)->next;
	return (chunk);
}

/*
Extracts all execution chunks from the input token list.
Calls ms_process_chunk for each chunk, storing the result in an array.
Allocates memory for the array dynamically based on the number of chunks.
Ensures safe cleanup of memory in case of processing failure.
Returns:
  - An array of strings representing chunks on success.
  - NULL on memory allocation failure.
*/
void	ms_extract_chunks(t_ms *ms, t_token **tokens)
{
	t_token	*current;
	int		i;
	int		count;
	int		start;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	ms->chunk_count = ms_count_chunks(ms, current);
	ms->exec_chunks = malloc(sizeof(t_token *) * ms->chunk_count);
	if (!ms->exec_chunks)
		return (ms_error_handler(ms, "Error: Mem alloc failed", 1));
	start = 0;
	i = 0;
	while (i < ms->chunk_count && current)
	{
		count = 0;
		while (current && current->type != T_PIPE && ++count)
			current = current->next;
		ms->exec_chunks[i] = ms_toksub(ms->chain_tokens, start, count);
		start += count + 1;
		i++;
		if (current && current->next)
			current = current->next;
	}
}

/*
Initializes the execution environment for the Minishell.
Breaks down input tokens into execution chunks and rebuilds the env array.
Calculates the number of pipes based on the chunk count.
Creates the necessary pipes for inter-process communication.
*/
void	ms_initialize_execution(t_ms *ms, char ***env)
{
	ms_extract_chunks(ms, &ms->chain_tokens);
	*env = ms_rebuild_env(ms);
	ms->pipe_count = ms_count_chunks(ms, ms->chain_tokens) - 1;
	ms_create_pipes(ms, &ms->pipe_fds, ms->pipe_count);
}
