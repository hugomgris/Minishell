/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chainer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/09 16:59:27 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_free_chains(t_ms *ms)
{
	t_chain	*current;
	t_chain	*next;

	current = *ms->chains;
	while (current)
	{
		next = current->next;
		ms_tokclear(&current->tokens, free);
		free(current->separator);
		free(current);
		current = next;
	}
	free(ms->chains);
}

int	ms_count_chains(t_ms *ms)
{
	t_token	*current;
	int		count;

	current = ms->tok;
	count = 1;
	while (current)
	{
		if (current->type == T_AND || current->type == T_OR)
			count++;
		current = current->next;
	}
	return (count);
}

void	ms_create_chain(t_ms *ms, int iter, int index)
{
	t_token	*current;
	int		start;
	int		count;

	current = ms->tok;
	start = 0;
	while (current && iter)
	{
		if (current->type == T_AND || current->type == T_OR)
			iter--;
		start++;
		current = current->next;
	}
	count = 0;
	while (current && current->type != T_AND && current->type != T_OR)
	{
		count++;
		current = current->next;
	}
	ms->chains[index] = malloc(sizeof(t_chain));
	if (!ms->chains[index])
		return (ms_error_handler(ms, "Error: mem alloc failed", 1));
	ms->chains[index]->tokens = ms_toksub(ms->tok, start, count);
	ms->chains[index]->separator = NULL;
	ms->chains[index]->next = NULL;
}

void	ms_build_chains(t_ms *ms)
{
	int	i;

	ms->chain_count = ms_count_chains(ms);
	ft_printf("count: %d\n", ms->chain_count);
	ms->chains = malloc(sizeof(t_chain *) * ms->chain_count);
	if (!ms->chains)
		return (ms_error_handler(ms, "Error: mem alloc failed", 1));
	i = -1;
	while (++i < ms->chain_count)
	{
		ms_create_chain(ms, i, i);
		if (i > 0)
			ms->chains[i - 1]->next = ms->chains[i];
		ft_printf("new chain contains:\n");
		ms_print_toks(ms->chains[i]->tokens);
	}
}
