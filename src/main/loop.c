/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 14:47:33 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_handle_input(t_ms *ms)
{
	ms->input = ms_check_empty_input(ms, ms->input);
	if (!ms->input)
		return ;
	if (!ms_syntax_checker(ms, ms->input) || !ms_tokenizer(ms, ms->input))
	{
		ms->exit_status = 2;
		add_history(ms->input);
		write_history(0);
		return ;
	}
	ms_build_chains(ms);
	add_history(ms->input);
	write_history(0);
}

void	ms_execute_chains(t_ms *ms)
{
	t_chain	*current;

	current = ms->chains;
	while (current)
	{
		ms->chain_tokens = current->tokens;
		ms_parser(ms);
		ms_executor(ms);
		current = current->next;
		if (current)
			ms_manage_separator(ms, &current);
	}
	ms_chain_clear(&ms->chains);
	ms_tokclear(&ms->tok, free);
	ms->chains = NULL;
}

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
		ms_handle_input(ms);
		ms_execute_chains(ms);
	}
}
