/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/08 14:35:33 by hmunoz-g         ###   ########.fr       */
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
	t_list	*current;

	ms->chains = 1;
	current = ms->tokens;
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
	t_list	*current;
	int		count;

	current = ms->tokens;
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

int	ms_and(t_list **tokens, int *i)
{
	int		count;
	t_list	*current;

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

int	ms_or(t_list **tokens, int *i)
{
	int		count;
	t_list	*current;

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
	t_list	*current;

	ms_count_chains(ms);
	i = -1;
	start = 0;
	current = ms->tokens;
	while (++i < ms->chains)
	{
		if (ms_get_set(GET, 0) > 1)
		{
			ft_lstclear(&ms->tokens, free);
			return ;
		}
		count = ms_get_chain_count(ms, i);
		ms->chain_tokens = ft_lstsub(ms->tokens, start, count);
		l = ft_lstsize((ms->chain_tokens));
		if (l == 0)
			break ;
		while (current && --l >= 0)
			current = current->next;
		status = ms_executor(ms);
		if (!status && current)
			start += count + ms_and(&current, &i);
		else if ((status == 1 || status == 2) && current)
			start += count + ms_or(&current, &i);
		ft_lstclear(&ms->chain_tokens, free);
	}
	ft_lstclear(&ms->tokens, free);
}
