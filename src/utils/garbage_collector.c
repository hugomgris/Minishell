/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:37:07 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 17:21:07 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
This is the Garbage Collector hub. It is a simple tool that:
	-Manages the GC list as a static t_list object
	-keeps track of dynamically allocated resources
	-Cleans up after itself with a function called before every exit.
*/

void	gc_cleanup(void)
{
	t_list	**gc;

	gc = gc_instance();
	if (gc && *gc)
		ft_lstclear(gc, free);
}

t_list	**gc_instance(void)
{
	static t_list	*gc = NULL;

	return (&gc);
}

void	gc_add(void	*ptr)
{
	t_list	**gc;
	t_list	*new_node;
	t_list	*tmp;

	gc = gc_instance();
	tmp = *gc;
	while (tmp)
	{
		if (tmp->content == ptr)
			return ;
		tmp = tmp->next;
	}
	new_node = ft_lstnew(ptr);
	if (!new_node)
		ms_error_handler("Memory allocation error", 1);
	ft_lstadd_back(gc, new_node);
}
