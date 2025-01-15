/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:10:37 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/14 15:07:23 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Iterates over a list and checks for potential empty tokens. If a token
	is empty, it is freed and the pointer from the previous token redirected to
	the next token, or NULL if at the end of the list.
*/
void	ms_remove_empty_tokens(t_token **lst, void (*del)(void *))
{
	t_token	*current;
	t_token	*previous;

	current = *lst;
	previous = NULL;
	while (current)
	{
		if (current->content && ms_is_empty_token(current->content))
		{
			ms_remove_token(lst, previous, current, del);
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

/*
	Checks if a token is an empty string.
	Useful to check after the variable expansion has occured.
*/
int	ms_is_empty_token(void *content)
{
	return (ft_strncmp((char *)content, "", 1) == 0);
}

/*
	Removes a token from a list and frees it.
*/
void	ms_remove_token(t_token **lst, t_token *prev, \
	t_token *cur, void (*del)(void *))
{
	if (!lst || !cur)
		return ;
	if (prev)
		prev->next = cur->next;
	else if (*lst)
		*lst = cur->next;
	if (cur->content)
		del(cur->content);
	free(cur);
}

int	ms_add_node_to_sublist(t_token **sub_list, t_token *current)
{
	char	*copy;
	t_token	*new_node;

	copy = ft_strdup(current->content);
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
