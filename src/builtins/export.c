/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:20:34 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/17 14:52:21 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function that prints the export-ready variables in ms_env.
The output is formatted following Bash's way of handling the export command.
Works similarly as ENV command, but also prints variables w/o values
*/
void	ms_export_print(t_ms *ms)
{
	t_list	*current;
	t_list	*sorted_env;
	char	*sym;
	char	*out;

	sorted_env = ft_lstcpy(ms->ms_env);
	sorted_env = ms_sort(sorted_env, ft_memcmp);
	current = ms_sort(sorted_env, ft_memcmp);
	while (current)
	{
		sym = ft_strchr(current->content, '=');
		if (sym)
			out = ms_build_export_output(ms, current->content, sym);
		else
		{
			out = ft_strdup(current->content);
			gc_add(out, &ms->gc);
		}
		ft_printf("declare -x %s\n", out);
		current = current->next;
	}
	ft_lstclear(&sorted_env, free);
}

/*
Flow control function that checks key syntax to comply with export rules
*/
int	ms_export_check(const char *var)
{
	int	i;

	i = 0;
	if (var[0] == '\0' || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (0);
	while (var[i] != '\0')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/*
Helper function that handles the export error output via ms_error_handler
*/
void	ms_export_error(t_ms *ms, char *entry)
{
	char	*output;

	output = "export: ";
	output = ft_strjoin(output, entry);
	gc_add(output, &ms->gc);
	output = ft_strjoin(output, ": Not a valid identifier");
	gc_add(output, &ms->gc);
	ms_error_handler(ms, output, 0);
}

/*
Executes the export command with variables.
Handles required cases:
	-If both a key and a value are input (i.e., arg has an '=')
		changes or creates the variable.
	-If there is no value asigned in input
		changes or creates the variable without an assigned val.

*/
void	ms_export_ex(t_ms *ms, char *key, char *value)
{
	if (value)
	{
		if (ms_key_exists(ms, key))
			ms_set_env_variable(ms, key, value);
		else
			ms_add_env_variable(ms, key, value);
	}
	else
	{
		if (!ms_key_exists(ms, key))
			ms_add_env_variable(ms, key, NULL);
	}
}

/*
EXPORT builtin main handler.
If there are no arguments, it calls the export print option.
Else, it processes each argument through exec fuction.
*/
void	ms_export(t_ms *ms)
{
	t_list	*current;
	t_list	*tokens;

	tokens = ms->filtered_tokens;
	if (!tokens->next)
	{
		ms_export_print(ms);
		return ;
	}
	current = tokens->next;
	while (current)
	{
		ms_process_export_arg(ms, current->content);
		current = current->next;
	}
}
