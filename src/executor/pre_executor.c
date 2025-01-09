/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/09 12:34:14 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Counts the number of chains of commands separated by '&&' or '||'.
It iterates through the tokens, and for each occurrence of '&&' or '||',
	the chain count is incremented.
The total number of chains is stored in the chains field inside ms struct.
*/
void	ms_count_chains(t_ms *ms)
{
	t_token	*current;

	ms->chains = 1;
	current = ms->tok;
	while (current)
	{
		if (!ft_strncmp(current->content, "&&", 2)
			|| !ft_strncmp(current->content, "||", 2))
			ms->chains++;
		current = current->next;
	}
}

/*
Gets the count of tokens in the current chain, based on it's iteration index.
The function iterates through the tokens until the specified index is reached,
then counts how many tokens exist in the chain before finding another separator.
*/
int	ms_get_chain_count(t_ms *ms, int iter)
{
	t_token	*current;
	int		count;

	current = ms->tok;
	count = 0;
	while (current && iter)
	{
		if (!ft_strncmp(current->content, "&&", 2)
			|| !ft_strncmp(current->content, "||", 2))
			iter--;
		current = current->next;
	}
	while (current && ft_strncmp(current->content, "&&", 2)
		&& ft_strncmp(current->content, "||", 2))
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	ms_and(t_token **tokens, int *i)
{
	int		count;
	t_token	*current;

	count = 0;
	current = *tokens;
	while (current && ft_strncmp(current->content, "&&", 2))
	{
		if (!ft_strncmp(current->content, "||", 2))
			*i += 1;
		current = current->next;
		count++;
	}
	if (current)
		current = current->next;
	*tokens = current;
	return (count + 1);
}

int	ms_or(t_token **tokens, int *i)
{
	int		count;
	t_token	*current;

	count = 0;
	current = *tokens;
	while (current && ft_strncmp(current->content, "||", 2))
	{
		if (!ft_strncmp(current->content, "&&", 2))
			*i += 1;
		current = current->next;
		count++;
	}
	if (current)
		current = current->next;
	*tokens = current;
	return (count + 1);
}

/*
Prepares and executes each chain of commands.
It first counts the number of chains, then iterates over each chain,
	executing the commands in the chain using ms_executor. 
The status of each chain is checked to determine if the next chain
	should be executed based on the logic of '&&' and '||'.
*/
void	ms_pre_executor(t_ms *ms)
{
	int		i;
	int		start;
	int		count;
	int		l;
	int		status;
	t_token	*current;

	ms_count_chains(ms);
	i = -1;
	start = 0;
	current = ms->tok;
	while (++i < ms->chains)
	{
		if (ms_get_set(GET, 0) > 1)
		{
			ms_tokclear(&ms->tok, free);
			return ;
		}
		count = ms_get_chain_count(ms, i);
		ms->chain_tokens = ms_toksub(ms->tok, start, count);
		l = ms_toksize((ms->chain_tokens));
		if (l == 0)
			break ;
		while (current && --l >= 0)
			current = current->next;
		status = ms_executor(ms);
		if (!status && current)
			start += count + ms_and(&current, &i);
		else if ((status == 1 || status == 2) && current)
			start += count + ms_or(&current, &i);
		ms_tokclear(&ms->chain_tokens, free);
	}
	ms_tokclear(&ms->tok, free);
}
