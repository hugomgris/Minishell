/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:30:34 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/02 20:48:34 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last;
	char		*start;

	if (str)
		last = str;
	if (!last)
		return (NULL);
	while (*last && strchr(delim, *last))
		last++;
	if (!*last)
		return (NULL);
	start = last;
	while (*last && !strchr(delim, *last))
		last++;
	if (*last)
	{
		*last = '\0';
		last++;
	}
	return (start);
}
