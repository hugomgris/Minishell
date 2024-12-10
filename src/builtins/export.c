/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:20:34 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/10 18:04:05 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_export_print(t_ms *ms)
{
	t_list	*current;

	current = ms->ms_env;
	current = ms_sort(current, ft_memcmp);
	while (current)
	{
		ft_printf("declare -x ");
		ft_printf("%s\n", current->content);
		current = current->next;
	}
}

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

void	ms_export_ex(t_ms *ms, char *key, char *value)
{
	char	*fkey;
	char	*entry;

	if (value)
	{
		fkey = ft_strjoin(key, "=");
		gc_add(fkey, &ms->gc);
		if (ms_get_env_variable(ms, fkey))
			ms_set_env_variable(ms, key, value);
		else
		{
			entry = ft_strjoin(fkey, value);
			gc_add(entry, &ms->gc);
			ms_add_env_variable(ms, entry);
		}
	}
	else
	{
		if (ms_get_env_variable(ms, key))
			ms_set_env_variable(ms, key, "");
		else
			ms_add_env_variable(ms, key);
	}
}

void	ms_export(t_ms *ms, char **input)
{
	int	i;

	i = 0;
	if (!input[1])
	{
		ms_export_print(ms);
		return ;
	}
	while (input[i])
	{
		ms_process_export_arg(ms, input[i]);
		i++;
	}
}
