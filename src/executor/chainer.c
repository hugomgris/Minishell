/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chainer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 12:19:30 by hmunoz-g         ###   ########.fr       */
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

int	ms_count_tokens(t_token *current)
{
	int	count;

	count = 0;
	while (current && current->type != T_AND && current->type != T_OR)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	ms_find_start_position(t_token **current, int iter, int id, char **sep)
{
	int	start;

	start = 0;
	if (id == 0)
		*sep = NULL;
	while (*current && iter)
	{
		if ((*current)->type == T_AND || (*current)->type == T_OR)
			iter--;
		start++;
		if (!iter)
			*sep = ft_strdup((*current)->content);
		*current = (*current)->next;
	}
	return (start);
}

t_token	*ms_extract_chain_tokens(t_ms *ms, char **sep, int iter, int index)
{
	t_token	*current;
	int		start;
	int		count;

	current = ms->tok;
	start = ms_find_start_position(&current, iter, index, sep);
	count = ms_count_tokens(current);
	return (ms_toksub(ms->tok, start, count));
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
