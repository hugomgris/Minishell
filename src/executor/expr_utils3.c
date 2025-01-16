/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/16 09:31:06 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Duplicates a token, creating a new token with the same content and type.
- Allocates memory for the new token.
- Duplicates the content of the original token.
- Copies the type of the original token.
- Sets the `next` pointer of the new token to `NULL`.
Returns the new token, or `NULL` if allocation fails.
*/
t_token	*ms_token_dup(t_token *token)
{
	t_token	*new;

	if (!token)
		return (NULL);
	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->content = ft_strdup(token->content);
	new->type = token->type;
	new->next = NULL;
	return (new);
}

/*
Finds the position of the matching closing parenthesis
	for a given opening parenthesis.
- Starts searching from the token after the opening parenthesis (`start->next`).
- Uses a counter to track nested parentheses.
- Returns the position of the matching closing parenthesis.
If no matching parenthesis is found, returns total number of tokens processed.
*/
int	ms_find_matching_paren(t_token *start)
{
	int		count;
	int		pos;
	t_token	*current;

	count = 1;
	pos = 0;
	current = start->next;
	while (current && count > 0)
	{
		if (current->type == T_LPARENTH)
			count++;
		else if (current->type == T_RPARENTH)
			count--;
		pos++;
		if (count == 0)
			return (pos);
		current = current->next;
	}
	return (pos);
}

/*
Frees an expression tree recursively.
- Frees the child subtree (if it exists).
- Frees the next subtree (if it exists).
- Clears the token list (if it exists).
- Frees the separator string (if it exists).
- Frees the current expression node.
*/
void	ms_free_expression_tree(t_expr *expr)
{
	if (!expr)
		return ;
	if (expr->child)
		ms_free_expression_tree(expr->child);
	if (expr->next)
		ms_free_expression_tree(expr->next);
	if (expr->tokens)
		ms_tokclear(&expr->tokens, free);
	if (expr->separator)
		free(expr->separator);
	free(expr);
}
