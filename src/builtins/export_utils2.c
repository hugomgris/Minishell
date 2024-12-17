/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:59:38 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/12 10:47:51 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function that cheks if ms_env has an entry with argument key.
*/
int	ms_key_exists(t_ms *ms, char *key)
{
	t_list	*current;

	current = ms->ms_env;
	while (current)
	{
		if (!ft_strncmp(current->content, key, ft_strlen(key)))
			return (1);
		current = current->next;
	}
	return (0);
}
