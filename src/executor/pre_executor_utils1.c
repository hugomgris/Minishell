/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_executor_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 09:25:58 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ms_add_node_to_sublist(t_token **sub_list, t_token *current)
{
	char	*copy;
	t_token	*new_node;

	copy = (current->content);
	if (!copy)
		return (0);
	new_node = ms_new_token(copy, current->type);
	if (!new_node)
	{
		free(copy);
		return (0);
	}
	ms_tokadd_back(sub_list, new_node);
	return (1);
}

t_token	*ms_toksub(t_token *lst, int start, int count)
{
	t_token	*sub_list;
	t_token	*current;
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
		if (!ms_add_node_to_sublist(&sub_list, current))
		{
			ms_tokclear(&sub_list, free);
			return (NULL);
		}
		current = current->next;
	}
	return (sub_list);
}
