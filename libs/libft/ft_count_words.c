/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:24:46 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/07 08:54:40 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_count_words(const char *str)
{
	int		count;
	bool	in_word;

	count = 0;
	in_word = false;
	if (!str)
		return (0);
	while (*str)
	{
		if (!ft_isspace(*str) && !in_word)
		{
			in_word = true;
			count++;
		}
		else if (ft_isspace(*str))
		{
			in_word = false;
		}
		str++;
	}
	return (count);
}
