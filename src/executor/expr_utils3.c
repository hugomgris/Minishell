/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/13 12:28:01 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
