/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/05 18:59:05 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to handle empty user input.
Trims leading and treading spaces, cleaning the input.
*/
char	*ms_check_empty_input(t_ms *ms, char *input)
{
	char	*trimmed;

	if (input == NULL)
		return (NULL);
	trimmed = ft_strtrim(input, " \n");
	if (!trimmed)
		ms_error_handler(ms, "Error: Memory allocation failed", 1);
	else
		gc_add(trimmed, &ms->gc);
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
void	ms_main_loop(t_ms *ms)
{
	static char	*prompt;

	while (42)
	{
		prompt = ms_build_prompt(ms);
		gc_add(prompt, &ms->gc);
		ms->input = readline(prompt);
		gc_add(ms->input, &ms->gc);
		if (ms->input == NULL)
		{
			ms_exit_handler(ms, "exit");
			break ;
		}
		ms->input = ms_check_empty_input(ms, ms->input);
		if (!ms->input)
			continue ;
		if (ft_strlen(ms->input) == 4 && !ft_strncmp(ms->input, "exit", 4))
			ms_exit_handler(ms, "exit");
		add_history(ms->input);
		if (!ms_parser(ms, ms->input))
			continue ;
		ms_executor(ms);
		gc_add(ms->input, &ms->gc);
	}
}
