/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr_n.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 18:00:29 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/04 18:18:47 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr_n(const char *s, int c, int n)
{
	size_t			len;
	int				count;
	unsigned char	target;

	count = 0;
	target = (unsigned char)c;
	len = 0;
	while (s[len] != '\0')
		len++;
	while (len > 0)
	{
		len--;
		if (s[len] == target)
		{
			count++;
			if (count == n)
				return ((char *)(s + len));
		}
	}
	return (NULL);
}
