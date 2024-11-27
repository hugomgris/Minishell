/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/27 18:48:50 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Small exit handler that prints an exit msg and exits.
If needed later on, we can move this to it's own file in utils.
*/
void	ms_exit_handler(const char *msg)
{
	if (msg)
		ft_printf("%s\n", msg);
	exit(0);
}

/*
Helper function to handle empty user input.
Trims leading and treading spaces, cleaning the input.
*/
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

/*
Main loop for Minishell.
Uses readline() to get input and adds it to history.
Handles graceful exits and empty input. 
For now, it just prints back the input.
*/
void	ms_main_loop(void)
{
	char	*input;

	while (42)
	{
		input = readline("minishell$ ");
		input = ms_check_empty_input(input);
		if (!input)
			continue ;
		if (ft_strlen(input) == 4 && !ft_strncmp(input, "exit", 4))
		{
			free(input);
			ms_exit_handler("exit");
		}
		add_history(input);
		ms_tokenizer(input);
		free(input);
	}
}
