/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:42:02 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/20 13:36:55 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1);
	if (result == NULL)
		return (NULL);
	if (s1 != NULL)
		ft_strlcpy(result, s1, len1 + 1);
	else
		result[0] = '\0';
	if (s2 != NULL)
		ft_strlcpy(result + len1, s2, len2 + 1);
	if (s3 != NULL)
		ft_strlcpy(result + len1 + len2, s3, len3 + 1);
	return (result);
}
