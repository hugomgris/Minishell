/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/31 12:14:05 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Converts the Minishell's internal environment linked list to an array of strings.
Steps:
  1. Iterates through the ms_env linked list, which stores environment variables.
  2. For each node containing an '=' character, dups content into the array.
  3. Handles memory allocation failures (ft_free and NULL return).
  4. Null-terminates the array.
  5. Returns pointer to array on success.
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

/*
Rebuilds the env array from the Minishell's internal linked list representation.
Steps:
  1. Allocates memory, accounting for one extra slot for null termination.
  2. Converts the ms_env linked list into the array using ms_env_to_array.
  3. Handles memory allocation or conversion failures.
  4. Returns pointer to the rebuilt env array on success.
*/
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
