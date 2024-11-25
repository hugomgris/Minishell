/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/25 17:03:08 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_main_loop(void)
{
	char	*input;
	char	*trimmed_input;

	while (1)
	{
		ft_printf("minishell> ");
		input = get_next_line(0);
		if (!input)
			exit(1);
		trimmed_input = ft_strtrim(input, " \n");
		free(input);
		if (!trimmed_input)
			exit (1);
		ft_printf("%s\n", trimmed_input);
		if (!ft_strncmp(trimmed_input, "exit", 4))
		{
			free(trimmed_input);
			exit(1);
		}
		free(trimmed_input);
	}
}
