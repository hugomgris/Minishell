/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/16 13:47:28 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_free_partial_array(char **arr, int index)
{
	while (index > 0)
		free(arr[--index]);
	free(arr);
}

char	**ms_allocate_env_array(t_list *list)
{
	int		count;
	char	**arr;

	count = ft_lstsize(list);
	arr = malloc(sizeof(char *) * (count + 1));
	return (arr);
}

/*
Re-creates an array of env variables from ms_env linked list initial copy.
It is needed to send to child processes as their env array.
*/
char	**ms_list_to_array(t_ms *ms, char **arr)
{
	t_list	*current;
	int		i;

	(void)ms;
	current = ms->ms_env;
	i = 0;
	while (current)
	{
		arr[i] = ft_strdup((char *)current->content);
		if (!arr[i])
		{
			ms_free_partial_array(arr, i);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
