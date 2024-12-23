/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/20 15:30:41 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains helper/Flow-control functions to build exec components:
	-ms_env_to_aray
	-ms_rebuild_env
*/

#include "../../includes/minishell.h"

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

char	**ms_rebuild_env(t_ms *ms)
{
	char	**arr;

	arr = (char **)malloc(sizeof(char *) * (ft_lstsize(ms->ms_env) + 1));
	if (!arr)
		return (ms_error_handler(ms, "Error: Mem alloc failed", 1), NULL);
	arr = ms_env_to_array(ms, arr);
	if (!arr)
		return (ms_error_handler(ms, "Failed to prepare environment", 0), NULL);
	return (arr);
}
