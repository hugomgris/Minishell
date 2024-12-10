/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:37:07 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/09 18:35:02 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
This is the Garbage Collector hub. It is a simple tool that:
	-Manages the GC list as a static t_list object
	-keeps track of dynamically allocated resources
	-Cleans up after itself with a function called before every exit.
*/

void	ms_print_list(t_list *list)
{
	while (list)
	{
		ft_printf("%s\n", (char *)list->content);
		list = list->next;
	}
}

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
