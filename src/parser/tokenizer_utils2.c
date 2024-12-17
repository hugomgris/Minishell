/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:10:37 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/17 11:15:17 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_remove_empty_tokens(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*previous;

	current = *lst;
	previous = NULL;
	while (current)
	{
		if (current->content && is_empty_token(current->content))
		{
			remove_token(lst, previous, current, del);
			if (previous)
				current = previous->next;
			else
				current = *lst;
		}
		else
		{
			previous = current;
			current = current->next;
		}
	}
}

int	is_empty_token(void *content)
{
	return (ft_strncmp((char *)content, "", 1) == 0);
}

void	remove_token(t_list **lst, t_list *prev, \
	t_list *cur, void (*del)(void *))
{
	if (prev)
		prev->next = cur->next;
	else
		*lst = cur->next;
	del(cur->content);
	free(cur);
}
