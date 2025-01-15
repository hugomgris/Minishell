/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/13 17:56:06 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_skip_similar_operators(t_expr **expr, char *op)
{
	while ((*expr)->next && (*expr)->next->separator
		&& !ft_strcmp((*expr)->next->separator, op))
		*expr = (*expr)->next;
}

int	ms_handle_and(t_ms *ms, t_expr *expr, int result)
{
	if (!result && expr->next)
		return (ms_execute_expression(ms, expr->next));
	ms_skip_similar_operators(&expr, "&&");
	if (expr->next && expr->next->next)
		return (ms_execute_expression(ms, expr->next->next));
	return (result);
}

int	ms_handle_or(t_ms *ms, t_expr *expr, int result)
{
	if (result && expr->next)
		return (ms_execute_expression(ms, expr->next));
	ms_skip_similar_operators(&expr, "||");
	if (expr->next && expr->next->next)
		return (ms_execute_expression(ms, expr->next->next));
	return (result);
}

int	ms_execute_expression(t_ms *ms, t_expr *expr)
{
	int	result;

	if (!expr || (!expr->tokens && !expr->next && !expr->child))
		return (1);
	if (expr->child)
		result = ms_execute_expression(ms, expr->child);
	else
	{
		ms->chain_tokens = expr->tokens;
		ms_parser(ms);
		if (ms_toksize(ms->chain_tokens) == 0)
			expr->tokens = ms->chain_tokens;
		ms_executor(ms);
		result = ms->exit_status;
	}
	if (!expr->separator)
		return (result);
	if (!ft_strcmp(expr->separator, "&&"))
		return (ms_handle_and(ms, expr, result));
	if (!ft_strcmp(expr->separator, "||"))
		return (ms_handle_or(ms, expr, result));
	return (result);
}
