/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:37:07 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 09:10:27 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to print lists.
Mainly used for debugging.
*/
void	ms_print_list(t_list *list)
{
	while (list)
	{
		ft_printf("%s\n", (char *)list->content);
		list = list->next;
	}
}

/*
	Same as ms_print_list, but for tokens.
*/
void	ms_print_toks(t_token *list)
{
	while (list)
	{
		ft_printf("%s, add:%p, %i\n", (char *)list->content, \
			list->content, list->type);
		list = list->next;
	}
}

/*
Main garbage collector function.
Adds an entry to the gc list inside ms minishell struct.
Before adding, checks if argument address is already store to avoid double frees.
*/
void	gc_add(void	*ptr, t_list **gc)
{
	t_list	*tmp;

	if (!ptr || !gc)
		return ;
	tmp = *gc;
	while (tmp)
	{
		if (tmp->content == ptr)
			return ;
		tmp = tmp->next;
	}
	ft_lstadd_back(gc, ft_lstnew(ptr));
}
