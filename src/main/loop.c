/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/30 16:44:58 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to handle empty user input.
Trims leading and treading spaces, cleaning the input.
*/
char	*ms_check_empty_input(char *input, t_list **ms_env, t_list **gc)
{
	char	*trimmed;

	if (input == NULL)
		return (NULL);
	trimmed = ft_strtrim(input, " \n");
	if (!trimmed)
		ms_error_handler(ms_env, gc, "Error: Memory allocation failed", 1);
	else
		gc_add(trimmed, gc);
	if (trimmed[0] == '\0')
		return (NULL);
	return (trimmed);
}

/*
Main loop for Minishell.
Calls for prompt build. 
Uses readline() to get input and adds it to history.
Handles graceful exits and empty input. 
For now, it just prints back the input.
*/
void	ms_main_loop(t_list **ms_env, t_list **gc)
{
	char			*input;
	static char		*prompt;

	while (42)
	{
		prompt = ms_build_prompt(ms_env, gc);
		gc_add(prompt, gc);
		input = readline(prompt);
		gc_add(input, gc);
		if (input == NULL)
		{
			ms_exit_handler(ms_env, gc, "exit");
			break ;
		}
		input = ms_check_empty_input(input, ms_env, gc);
		if (!input)
			continue ;
		if (ft_strlen(input) == 4 && !ft_strncmp(input, "exit", 4))
			ms_exit_handler(ms_env, gc, "exit");
		add_history(input);
		ms_tokenizer(ms_env, input, gc);
		gc_add(input, gc);
	}
}
