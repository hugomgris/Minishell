/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 14:52:43 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_and(t_chain **chain)
{
	t_chain	*current;

	current = *chain;
	while (current && current->separator && ft_strcmp(current->separator, "&&"))
		current = current->next;
	*chain = current;
}

void	ms_or(t_chain **chain)
{
	t_chain	*current;

	current = *chain;
	while (current && current->separator && ft_strcmp(current->separator, "||"))
		current = current->next;
	*chain = current;
}

void	ms_manage_separator(t_ms *ms, t_chain **chain)
{
	int		status;
	t_chain	*current;

	status = ms->exit_status;
	current = *chain;
	if (!status)
	{
		ms_and(&current);
		*chain = current;
		return ;
	}
	else
	{
		ms_or(&current);
		*chain = current;
		return ;
	}
}
