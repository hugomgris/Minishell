/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/06 10:04:04 by hmunoz-g         ###   ########.fr       */
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
	int		status;

	ms_count_chains(ms);
	i = -1;
	start = 0;
	status = 0;
	while (++i < ms->chains)
	{
		count = ms_get_chain_count(ms, i);
		ms->chain_tokens = ft_lstsub(ms->tokens, start, count);
		ms_executor(ms);
		status = ms->exit_status;
		if (!status && !ft_strncmp(ms->tokens->content, "&&", 2))
			start += (count + 1);
		else if (status && !ft_strncmp(ms->tokens->content, "||", 2))
			start += (count + 1);
		else
			start += (count + 1);
		ft_lstclear(&ms->chain_tokens, free);
	}
	ft_lstclear(&ms->tokens, free);
}
