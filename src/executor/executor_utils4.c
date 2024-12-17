/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/17 15:24:41 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to allocate the array rebuild of the env variables
	that will be sent to child processes.
*/
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
It only adds variables with a set value to the array.
(Empty set values can exist, as well as vars without value (i.e., no '=')).
*/
char	**ms_env_to_array(t_ms *ms, char **arr)
{
	t_list	*current;
	int		i;

	(void)ms;
	current = ms->ms_env;
	i = 0;
	while (current)
	{
		if (ft_strchr(current->content, '='))
		{
			arr[i] = ft_strdup((char *)current->content);
			if (!arr[i])
			{
				ft_free(arr);
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	arr[i] = NULL;
	return (arr);
}
