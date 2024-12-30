/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_executor_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 17:57:55 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	ms_get_chain_count(t_list **tokens)
{
	t_list	*current;
	int		count;

	current = *tokens;
	count = 0;
	while (current && (!ft_strncmp(current->content, "&&", 2)
			|| !ft_strncmp(current->content, "||", 2)))
	{
		count++;
		current = current->next;
	}
	*tokens = current->next;
	return (count);
}

void	ms_pre_executor(t_ms *ms)
{
	int		i;
	int		start;
	int		count;
	t_list	*current;

	ms_count_chains(ms);
	i = -1;
	count = 0;
	current = ms->tokens;
	while (++i < ms->chains)
	{
		start = count;
		count = ms_get_chain_count(&current);
		ms->chain_tokens = ft_lstsub(ms->tokens, start, count);
		ms_executor(ms);
		count++;
		ft_lstclear(&ms->chain_tokens, free);
	}
}
