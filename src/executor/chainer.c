/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chainer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 10:29:42 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

t_token	*ms_extract_chain_tokens(t_ms *ms, char **sep, int iter, int index)
{
	t_token	*current;
	t_token	*sub;
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
	if (index == 0)
		*sep = NULL;
	else
		*sep = ft_strdup(current->content);
	count = 0;
	while (current && current->type != T_AND && current->type != T_OR)
	{
		count++;
		current = current->next;
	}
	sub = ms_toksub(ms->tok, start, count);
	return (sub);
}

void	ms_build_chains(t_ms *ms)
{
	int		i;
	t_token	*aux;
	t_chain	*tmp;
	char	*sep;

	ms->chain_count = ms_count_chains(ms);
	i = 0;
	while (i < ms->chain_count)
	{
		aux = ms_extract_chain_tokens(ms, &sep, i, i);
		tmp = ms_new_chain(aux, sep);
		if (tmp == NULL)
			return (ms_error_handler(ms, "Error: Mem alloc failed", 1));
		ms_chain_add_back(&ms->chains, tmp);
		i++;
	}
}
