/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:20:34 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/13 15:07:15 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to check export builtin cmd argument's syntax/contents.
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
Helper function to output specicif export builtin cmd errors.
*/
int	ms_export_error(t_ms *ms, char *entry)
{
	char	*output;

	output = "export: ";
	output = ft_strjoin(output, entry);
	gc_add(output, &ms->gc);
	output = ft_strjoin(output, ": Not a valid identifier");
	gc_add(output, &ms->gc);
	ms_error_handler(ms, output, 0);
	return (1);
}

/*
Flow control function to check if export arguments pre-existance.
Calls for argument syntax check, returns error if not passed.
If passed:
	-If key exists, changes it's value.
	-If key does NOT exist, creates env/export entry.
	-If argument contains no value and key does NOT exist,
		creates a new entry key with no value (not set for export).
*/
int	ms_export_ex(t_ms *ms, char *key, char *value)
{
	t_list	*new_node;
	char	*export_key ;

	if (!ms_export_check(key))
		return (ms_export_error(ms, key));
	if (value)
	{
		if (ms_key_exists(ms, key))
			ms_set_env_variable(ms, key, value);
		else
			ms_add_env_variable(ms, key, value);
		ms_remove_from_export_only(ms, key);
	}
	else
	{
		if (!ms_key_exists(ms, key) && !ms_is_export_only(ms, key))
		{
			export_key = ft_strdup(key);
			gc_add(export_key, &ms->gc);
			new_node = ft_lstnew(export_key);
			gc_add(new_node, &ms->gc);
			ft_lstadd_back(&ms->export_only, new_node);
		}
	}
	return (0);
}

/*
EXPORT builtin command handler.
Checks arguments and derives to case managing:
	-no argument: prints export list
	-sent argument: calls processing functions
*/
int	ms_export(t_ms *ms, char **cmd_args, char **env)
{
	int		code;
	t_token	*current;

	if (!cmd_args[1])
		return (ms_export_print(ms, env));
	current = ms->chain_tokens->next;
	while (current)
	{
		code = ms_process_export_arg(ms, current->content);
		current = current->next;
	}
	return (code);
}
