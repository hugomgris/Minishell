/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/26 10:18:54 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_exit_handler(const char *msg)
{
	if (msg)
		ft_printf("%s\n", msg);
	exit(0);
}

char	*ms_check_empty_input(char *input)
{
	char	*trimmed;

	trimmed = ft_strtrim(input, " \n");
	free(input);
	if (!trimmed)
		ms_error_handler("Memory allocation error");
	if (trimmed[0] == '\0')
	{
		free(trimmed);
		return (NULL);
	}
	return (trimmed);
}

void	ms_main_loop(void)
{
	char	*input;

	while (1)
	{
		ft_printf("minishell$ ");
		input = get_next_line(0);
		if (!input)
			ms_exit_handler("exit");
		input = ms_check_empty_input(input);
		if (!input)
			continue ;
		if (!ft_strncmp(input, "exit", 4))
		{
			free(input);
			ms_exit_handler("exit");
		}
		ft_printf("%s\n", input);
		free(input);
	}
}
