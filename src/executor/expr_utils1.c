/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/16 09:28:34 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Extracts the content between parentheses from a token chain.
- Starts from the token after the opening parenthesis (`start->next`).
- Iterates through the tokens, duplicating and adding them to a new token list.
- Stops after processing `len - 1` tokens (to exclude the closing parenthesis).
Returns the new token list containing the content between parentheses.
*/
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

/*
Initializes a new expression node (`t_expr`).
- Allocates memory for the expression node.
- Initializes its fields (`tokens`, `next`, `child`, `separator`) to `NULL`.
Returns the initialized expression node, or `NULL` if allocation fails.
*/
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

/*
Handles parentheses in the token chain by extracting their content
	and building a subtree.
- Finds the length of the content between the parentheses
	using `ms_find_matching_paren`.
- Extracts the content using `ms_extract_paren_content`.
- Builds an expression subtree for the content using `ms_build_expression_tree`.
- Cleans up the extracted content tokens.
- Advances the `current` token pointer past the parentheses.
*/
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

/*
Processes a token and updates the expression tree accordingly.
- If the token is an opening parenthesis (`T_LPARENTH`),
	handles it using `ms_handle_parentheses`.
- If the token is a logical operator (`T_AND` or `T_OR`),
	sets the separator and builds the next part of the expression tree.
- For other tokens, adds them to the current expression's token list.
Advances the `current` token pointer as needed.
*/
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

/*
Builds an expression tree from a chain of tokens.
- Initializes a new expression node using `ms_init_expr`.
- Iterates through the tokens, processing each one using `ms_process_token`.
Returns the root of the expression tree.
*/
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
