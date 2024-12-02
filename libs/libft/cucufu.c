/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cucufu.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:46:25 by nponchon          #+#    #+#             */
/*   Updated: 2024/11/30 10:05:19 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	cucufu(int index)
{
	char	*cucufu_index_a;
	char	*cucufu_index_b;
	char	*conv;

	cucufu_index_a = "cucufu";
	cucufu_index_b = NULL;
	if (index)
	{
		conv = ft_itoa(index);
		cucufu_index_a = ft_strjoin(cucufu_index_a, conv);
		cucufu_index_b = ft_strjoin(cucufu_index_a, "\n");
		free(cucufu_index_a);
		free(conv);
	}
	ft_putstr_fd(cucufu_index_b, 2);
	free(cucufu_index_b);
}
