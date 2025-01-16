/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/16 09:30:09 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Skips consecutive logical operators of the same type in the expression tree.
- Advances the `expr` pointer to the next node
	if the next node's separator matches `op`.
- Used to handle chained logical operators (e.g., `cmd1 && cmd2 && cmd3`).
*/
void	ms_skip_similar_operators(t_expr **expr, char *op)
{
	while ((*expr)->next && (*expr)->next->separator
		&& !ft_strcmp((*expr)->next->separator, op))
		*expr = (*expr)->next;
}

/*
Handles the logical AND (`&&`) operator in the expression tree.
- If the previous command (`result`) succeeded (returned 0),
	executes the next command.
- Skips any consecutive `&&` operators.
- If there are more commands after the skipped operators, executes them.
Returns the result of the last executed command.
*/
int	ms_handle_and(t_ms *ms, t_expr *expr, int result)
{
	if (!result && expr->next)
		return (ms_execute_expression(ms, expr->next));
	ms_skip_similar_operators(&expr, "&&");
	if (expr->next && expr->next->next)
		return (ms_execute_expression(ms, expr->next->next));
	return (result);
}

/*
Handles the logical OR (`||`) operator in the expression tree.
- If the previous command (`result`) failed (returned non-zero),
	executes the next command.
- Skips any consecutive `||` operators.
- If there are more commands after the skipped operators, executes them.
Returns the result of the last executed command.
*/
int	ms_handle_or(t_ms *ms, t_expr *expr, int result)
{
	if (result && expr->next)
		return (ms_execute_expression(ms, expr->next));
	ms_skip_similar_operators(&expr, "||");
	if (expr->next && expr->next->next)
		return (ms_execute_expression(ms, expr->next->next));
	return (result);
}

/*
Executes an expression tree, handling commands,
	parentheses, and logical operators.
- If the expression is empty or invalid, returns 1.
- If the expression has a child (parentheses), recursively executes the child.
- Otherwise, parses and executes the command represented by the tokens.
- Handles logical operators (&&, ||) using `ms_handle_and` and `ms_handle_or`.
Returns the result of the executed command or expression.
*/
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
