/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsub_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:33:23 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/20 14:56:31 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	add_node_to_sublist(t_list **sub_list, t_list *current)
{
	char	*copy;
	t_list	*new_node;

	copy = ft_strdup(current->content);
	if (!copy)
		return (0);
	new_node = ft_lstnew(copy);
	if (!new_node)
	{
		free(copy);
		return (0);
	}
	ft_lstadd_back(sub_list, new_node);
	return (1);
}

t_list	*ft_lstsub(t_list *lst, int start, int count)
{
	t_list	*sub_list;
	t_list	*current;
	int		i;

	if (!lst || start < 0 || count <= 0)
		return (NULL);
	sub_list = NULL;
	current = lst;
	i = -1;
	while (current && ++i < start)
		current = current->next;
	i = -1;
	while (current && ++i < count)
	{
		if (!add_node_to_sublist(&sub_list, current))
		{
			ft_lstclear(&sub_list, free);
			return (NULL);
		}
		current = current->next;
	}
	return (sub_list);
}
