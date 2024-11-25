/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:24:46 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/10/15 13:18:56 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}
