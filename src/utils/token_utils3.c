/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:26:24 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/10 18:09:59 by nponchon         ###   ########.fr       */
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

/*
	Similar to ft_strcmp(), but ignores the lowercase/uppercase difference.
*/
int	ms_tokcmp(const char *s1, const char *s2)
{
	int		i;
	int		res;
	char	*cpy1;
	char	*cpy2;

	cpy1 = ft_strdup_lower(s1);
	cpy2 = ft_strdup_lower(s2);
	i = 0;
	res = 0;
	while (cpy1[i] && cpy2[i])
	{
		if (cpy1[i] != cpy2[i])
		{
			res = cpy1[i] - cpy2[i];
			free(cpy1);
			free(cpy2);
			return (res);
		}
		i++;
	}
	res = cpy1[i] - cpy2[i];
	free(cpy1);
	free(cpy2);
	return (res);
}

t_token	*ms_tokcpy(t_token *original)
{
	t_token	*new_list;
	t_token	*new_node;

	new_list = NULL;
	while (original)
	{
		new_node = ms_new_token(ft_strdup(original->content), original->type);
		if (!new_node)
			return (NULL);
		ms_tokadd_back(&new_list, new_node);
		original = original->next;
	}
	return (new_list);
}
