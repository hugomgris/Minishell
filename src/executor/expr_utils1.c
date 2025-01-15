/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/13 17:28:45 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ms_extract_paren_content(t_token *start, int len)
{
	t_token	*result;
	t_token	*current;
	int		i;

	result = NULL;
	current = start->next;
	i = 0;
	while (i < len - 1)
	{
		ms_tokadd_back(&result, ms_token_dup(current));
		current = current->next;
		i++;
	}
	return (result);
}

t_expr	*ms_init_expr(void)
{
	t_expr	*expr;

	expr = malloc(sizeof(t_expr));
	if (!expr)
		return (NULL);
	expr->tokens = NULL;
	expr->next = NULL;
	expr->child = NULL;
	expr->separator = NULL;
	return (expr);
}

void	ms_handle_parentheses(t_ms *ms, t_expr *expr, t_token **current)
{
	t_token	*paren_content;
	int		len;

	len = ms_find_matching_paren(*current);
	paren_content = ms_extract_paren_content(*current, len);
	expr->child = ms_build_expression_tree(ms, paren_content);
	ms_tokclear(&paren_content, free);
	*current = (*current)->next;
	while (len-- > 0)
		*current = (*current)->next;
}

void	ms_process_token(t_ms *ms, t_expr *expr, t_token **current)
{
	if ((*current)->type == T_LPARENTH)
		ms_handle_parentheses(ms, expr, current);
	else if ((*current)->type == T_AND || (*current)->type == T_OR)
	{
		expr->separator = ft_strdup((*current)->content);
		if ((*current)->next)
			expr->next = ms_build_expression_tree(ms, (*current)->next);
		*current = NULL;
	}
	else
	{
		ms_tokadd_back(&expr->tokens, ms_token_dup(*current));
		*current = (*current)->next;
	}
}

t_expr	*ms_build_expression_tree(t_ms *ms, t_token *tokens)
{
	t_expr	*expr;
	t_token	*current;

	expr = ms_init_expr();
	if (!expr)
		return (NULL);
	current = tokens;
	while (current)
		ms_process_token(ms, expr, &current);
	return (expr);
}
