/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_n.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 18:00:29 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/05 11:54:01 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr_n(const char *s, int c, int n)
{
	size_t			i;
	int				count;
	unsigned char	target;

	count = 0;
	i = 0;
	target = c;
	while (s[i] != '\0')
	{
		if (s[i] == target)
		{
			count++;
			if (count == n)
				return ((char *)(s + i));
		}
		i++;
	}
	return (NULL);
}
