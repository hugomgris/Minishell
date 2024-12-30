/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_executor_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 19:34:31 by hmunoz-g         ###   ########.fr       */
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

void	ms_pre_executor(t_ms *ms)
{
	int		i;
	int		start;
	int		count;
	int		iter;

	ms_count_chains(ms);
	i = -1;
	count = 0;
	start = 0;
	iter = 0;
	while (++i < ms->chains)
	{
		count = ms_get_chain_count(ms, iter);
		ms->chain_tokens = ft_lstsub(ms->tokens, start, count);
		ms_executor(ms);
		start += (count + 1);
		iter++;
		ft_lstclear(&ms->chain_tokens, free);
	}
	ft_lstclear(&ms->tokens, free);
}
