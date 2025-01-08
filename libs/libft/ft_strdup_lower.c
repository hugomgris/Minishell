/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_lower.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:35:56 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/07 12:44:39 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	Duplicates a string like ft_strdup() putting all uppercase letters
	to lowercases.
*/
char	*ft_strdup_lower(const char *str)
{
	char	*dup;
	int		i;

	i = 0;
	dup = (char *)malloc(sizeof(char) * (ft_strlen(str)) + 1);
	if (!dup)
		return (NULL);
	while (str[i])
	{
		dup[i] = ft_tolower(str[i]);
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
