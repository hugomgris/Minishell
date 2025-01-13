/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:25:37 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/13 17:25:53 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
    Inserts a new token into the list next to the specified current token.
    If current is NULL, the token is added at the beginning of the list.
*/
void	ms_tokinsert(t_token **lst, t_token *current, t_token *new)
{
	if (!lst || !new)
		return ;
	if (current == NULL)
	{
		new->next = *lst;
		*lst = new;
		return ;
	}
	new->next = current->next;
	current->next = new;
}

void	ms_tokclear(t_token **lst, void (*del)(void *))
{
	t_token	*tmp;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if (del)
			del((*lst)->content);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

t_token	*ms_toklast(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ms_tokadd_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ms_toklast(*lst);
	last->next = new;
}

/*
	Creates a new token, similar to the node from the original libft,
	with the addition of the token type.
*/
t_token	*ms_new_token(void *content, t_token_type type)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->content = content;
	new->type = type;
	new->next = NULL;
	return (new);
}
