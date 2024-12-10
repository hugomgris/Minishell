/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:libs/libft/ft_isdigit_str.c
/*   Created: 2024/12/09 13:57:58 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/09 13:59:25 by hmunoz-g         ###   ########.fr       */
=======
/*   Created: 2024/11/28 12:54:30 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/09 11:13:51 by nponchon         ###   ########.fr       */
>>>>>>> develop3:src/utils/exit_handler.c
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= 48 && str[i] <= 57))
			return (0);
		i++;
	}
	return (1);
}
