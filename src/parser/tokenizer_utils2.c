/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:10:37 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/13 18:10:41 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Iterates over a list and checks for potential empty tokens. If a token
	is empty, it is freed and the pointer from the previous token redirected to
	the next token, or NULL if at the end of the list.
*/
void	ms_remove_empty_tokens(t_token **lst, void (*del)(void *))
{
	t_token	*current;
	t_token	*previous;

	current = *lst;
	previous = NULL;
	while (current)
	{
		if (current->content && ms_is_empty_token(current->content))
		{
			ms_remove_token(lst, previous, current, del);
			if (previous)
				current = previous->next;
			else
				current = *lst;
		}
		else
		{
			previous = current;
			current = current->next;
		}
	}
}

/*
	Checks if a token is an empty string.
	Useful to check after the variable expansion has occured.
*/
int	ms_is_empty_token(void *content)
{
	return (ft_strncmp((char *)content, "", 1) == 0);
}

/*
	Removes a token from a list and frees it.
*/
void	ms_remove_token(t_token **lst, t_token *prev, \
	t_token *cur, void (*del)(void *))
{
	if (!lst || !cur)
		return ;
	if (prev)
		prev->next = cur->next;
	else if (*lst)
		*lst = cur->next;
	if (cur->content)
		del(cur->content);
	free(cur);
	cur = NULL;
}

/*
	Checks for potential redirectin with parenthesis, which is not handled
	by minishell.
*/
int	ms_check_redirparenthesis(t_ms *ms)
{
	t_token	*aux;

	aux = ms->tok;
	while (aux && aux->next)
	{
		if (ms_is_operator(aux->content) \
			&& (aux->next->type == T_LPARENTH || aux->next->type == T_RPARENTH))
		{
			ms_error_handler(ms, "unsupported redirection with parenthesis", 0);
			return (FALSE);
		}
		else if (ms_is_operator(aux->next->content) \
			&& (aux->type == T_LPARENTH || aux->type == T_RPARENTH))
		{
			ms_error_handler(ms, "unsupported redirection with parenthesis", 0);
			return (FALSE);
		}
		aux = aux->next;
	}
	return (TRUE);
}
