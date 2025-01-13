/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/13 12:10:56 by hmunoz-g         ###   ########.fr       */
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
	ms->expr_tree = ms_build_expression_tree(ms, ms->tok);
	if (!ms->expr_tree)
	{
		ms_error_handler(ms, "Error: Expression tree creation failed", 1);
		return ;
	}
	add_history(ms->input);
	write_history(0);
}

void	ms_execute_commands(t_ms *ms)
{
	if (ms->expr_tree)
	{
		ms_execute_expression(ms, ms->expr_tree);
		ms_free_expression_tree(ms->expr_tree);
		ms->expr_tree = NULL;
	}
	ms_tokclear(&ms->tok, free);
}

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
		ms_execute_commands(ms);
	}
}
