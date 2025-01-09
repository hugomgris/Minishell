/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/09 15:19:07 by hmunoz-g         ###   ########.fr       */
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
Catches CTRL+D input case (with null input check).
Passes the input to the tokenizer and calls the executor.
*/
void	ms_main_loop(t_ms *ms)
{
	t_chain	*current;

	while (42)
	{
		ms_get_set(SET, 0);
		ms->prompt = ms_build_prompt(ms);
		gc_add(ms->prompt, &ms->gc);
		ms->input = readline(ms->prompt);
		gc_add(ms->input, &ms->gc);
		if (ms->input == NULL)
		{
			ms_exit_handler(ms, "exit", 0);
			break ;
		}
		ms->input = ms_check_empty_input(ms, ms->input);
		if (!ms->input)
			continue ;
		if (!ms_syntax_checker(ms, ms->input))
			break ;
		if (ms_tokenizer(ms, ms->input))
			break ;
		ms_build_chains(ms); //TODO
		add_history(ms->input);
		write_history(0);
		current = ms->chains;
		while (current)
		{
			ms_parser(ms, current);
			ms_execute(ms);
			current = current->next;
		}
		//Clean chains
	}
}
