/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/25 16:16:09 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_main_loop(void)
{
	char	*input;

	while (1)
	{
		ft_printf("minishell> ");
		input = get_next_line(0);
		if (!input)
			exit(1);
		ft_printf("%s", input);
		if (!ft_strncmp(input, "exit", 4))
		{
			free(input);
			exit(1);
		}
		free(input);
	}
}
