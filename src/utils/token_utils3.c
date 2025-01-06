/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:26:24 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/06 17:41:26 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_toksize(t_token *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

/*
    Inserts a list of tokens into another list starting from the specified
	current token. If current is NULL, the token is added at the beginning
	of the list.
*/
void	ms_tokinsert_list(t_token **lst, t_token *current, t_token *new)
{
	t_token	*last;

	if (!lst || !new || !new->next)
		return ;
	if (current == NULL)
	{
		new->next = *lst;
		*lst = new;
		return ;
	}
	last = ms_toklast(new);
	last->next = current->next;
	current->next = new;
}
