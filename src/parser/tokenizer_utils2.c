/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:10:37 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/18 15:34:09 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ft_newtoken(void *content, t_token_type type)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->content = content;
	new->type = type;
	new->next = NULL;
	return (new);
}

/*
	Iterates over a list and checks for potential empty tokens. If a token
	is empty, it is freed and the pointer from the previous token redirected to
	the next token, or NULL if at the end of the list.
*/
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

/*
	Checks if a token is an empty string.
	Useful to check after the variable expansion has occured.
*/
int	is_empty_token(void *content)
{
	return (ft_strncmp((char *)content, "", 1) == 0);
}

/*
	Removes a token from a list and frees it.
*/
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
