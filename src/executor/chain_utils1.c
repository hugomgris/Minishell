/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chain_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 09:07:38 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_chain_clear(t_chain **lst)
{
	t_chain	*tmp;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		ms_tokclear(&(*lst)->tokens, free);
		if ((*lst)->separator)
			free((*lst)->separator);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

t_chain	*ms_chain_last(t_chain *lst)
{
	if (!lst)
		return (NULL);
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void	ms_chain_add_back(t_chain **lst, t_chain *new)
{
	t_chain	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ms_chain_last(*lst);
	if (last)
		last->next = new;
}

t_chain	*ms_new_chain(t_token *tokens, char *separator)
{
	t_chain	*new;

	new = (t_chain *)malloc(sizeof(t_chain));
	if (new == NULL)
		return (NULL);
	new->tokens = tokens;
	new->separator = separator;
	new->next = NULL;
	return (new);
}
