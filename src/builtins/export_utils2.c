/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:59:38 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/24 10:29:14 by hmunoz-g         ###   ########.fr       */
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

size_t	ft_min_strlen(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (len1 < len2)
		return (len1);
	else
		return (len2);
}
