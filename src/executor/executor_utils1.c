/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 15:44:06 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*int	ms_is_pipe(t_ms *ms, char *token)
{
	(void)ms;
	return (token && ft_strncmp(token, "|", 1) == 0);
}*/

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
char	**ms_extract_chunks(t_ms *ms, t_token **tokens)
{
	t_token	*current;
	char	**chunks;
	int		count;
	int		i;

	if (!tokens || !*tokens)
		return (NULL);
	current = *tokens;
	count = ms_count_chunks(ms, current);
	chunks = malloc(sizeof(char *) * (count + 1));
	if (!chunks)
		return (ms_error_handler(ms, "Error: Mem alloc failed", 1), NULL);
	i = 0;
	while (current && i < count)
	{
		chunks[i] = ms_process_chunk(ms, &current);
		if (!chunks[i])
		{
			ft_free(chunks);
			return (NULL);
		}
		i++;
	}
	chunks[i] = NULL;
	return (chunks);
}

/*
Initializes the execution environment for the Minishell.
Breaks down input tokens into execution chunks and rebuilds the env array.
Calculates the number of pipes based on the chunk count.
Creates the necessary pipes for inter-process communication.
*/
void	ms_initialize_execution(t_ms *ms, char ***env)
{
	ms->exec_chunks = ms_extract_chunks(ms, &ms->chain_tokens);
	*env = ms_rebuild_env(ms);
	ms->pipe_count = ft_array_count(ms->exec_chunks) - 1;
	ms_create_pipes(ms, &ms->pipe_fds, ms->pipe_count);
}
