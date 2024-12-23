/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/23 16:03:18 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
UNSET builtin command handler.
Searches for variable name in env list (ms_env).
If found, pops it out of the list (handling links).
*/

int	ms_unset_key_match(t_list *node, char *key)
{
	return (!ft_strncmp(node->content, key, ft_strlen(key))
		&& ((char *)node->content)[ft_strlen(key)] == '=');
}

void	ms_unset_remove_node(t_list **head, t_list *prev, t_list *current)
{
	if (prev == NULL)
		*head = current->next;
	else
		prev->next = current->next;
	ft_lstdelone(current, free);
}

void	ms_unset_env_key(t_list **env, char *key)
{
	t_list	*prev;
	t_list	*current;

	prev = NULL;
	current = *env;
	while (current)
	{
		if (ms_unset_key_match(current, key))
		{
			ms_unset_remove_node(env, prev, current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ms_unset(t_ms *ms)
{
	int	i;

	if (!ms->ms_env || !ms->filt_args[1])
		return (1);
	i = 1;
	while (ms->filt_args[i])
	{
		if (!ft_strchr(ms->filt_args[i], '='))
			ms_unset_env_key(&ms->ms_env, ms->filt_args[i]);
		i++;
	}
	return (0);
}
