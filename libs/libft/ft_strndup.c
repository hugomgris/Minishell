/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 09:13:41 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 16:18:21 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	copy_len;

	len = ft_strlen(s);
	if (n < len)
		copy_len = n;
	else
		copy_len = len;
	dup = (char *)malloc(copy_len + 1);
	if (!dup)
		return (NULL);
	strncpy(dup, s, copy_len);
	dup[copy_len] = '\0';
	return (dup);
}
