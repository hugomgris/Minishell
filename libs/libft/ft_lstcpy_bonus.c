/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstcpy_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:14:51 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/12 12:05:37 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstcpy(t_list *original)
{
	t_list	*new_list;
	t_list	*new_node;

	new_list = NULL;
	while (original)
	{
		new_node = ft_lstnew(ft_strdup(original->content));
		if (!new_node)
			return (NULL);
		ft_lstadd_back(&new_list, new_node);
		original = original->next;
	}
	return (new_list);
}
