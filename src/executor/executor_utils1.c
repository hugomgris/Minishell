/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/27 13:02:24 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains helper/Flow-control functions for exec chunk handling:
	-ms_is_pipe
	-ms_count_chunks
	-ms_process_chunk
	-ms_extract_chunks
*/

#include "../../includes/minishell.h"

int	ms_is_pipe(const char *token)
{
	return (token && ft_strncmp(token, "|", 1) == 0);
}

int	ms_count_chunks(t_list *tokens)
{
	int		count;
	t_list	*current;

	count = 1;
	current = tokens;
	while (current)
	{
		if (ms_is_pipe(current->content))
			count++;
		current = current->next;
	}
	return (count);
}

char	*ms_process_chunk(t_ms *ms, t_list **current)
{
	char	*chunk;
	char	*tmp;

	chunk = NULL;
	while (*current && !ms_is_pipe((*current)->content))
	{
		tmp = chunk;
		chunk = ft_strjoin3(chunk, " ", (*current)->content);
		if (!chunk)
		{
			ms_error_handler(ms, "Error: Mem alloc failed", 1);
			free(tmp);
			return (NULL);
		}
		free(tmp);
		*current = (*current)->next;
	}
	if (*current && ms_is_pipe((*current)->content))
		*current = (*current)->next;
	return (chunk);
}

char	**ms_extract_chunks(t_ms *ms, t_list **tokens)
{
	t_list	*current;
	char	**chunks;
	int		count;
	int		i;

	if (!tokens || !*tokens)
		return (NULL);
	current = *tokens;
	count = ms_count_chunks(current);
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

void	ms_initialize_execution(t_ms *ms, char ***env)
{
	ms->exec_chunks = ms_extract_chunks(ms, &ms->tokens);
	*env = ms_rebuild_env(ms);
	ms->pipe_count = ft_array_count(ms->exec_chunks) - 1;
	ms_create_pipes(ms, &ms->pipe_fds, ms->pipe_count);
}
