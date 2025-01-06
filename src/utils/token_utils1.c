/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:25:37 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/31 11:43:43 by hmunoz-g         ###   ########.fr       */
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

/*
	Same as ms_print_list, but for tokens.
*/
void	ms_print_toks(t_token *list)
{
	while (list)
	{
		ft_printf("%s, %i\n", (char *)list->content, list->type);
		list = list->next;
	}
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
