/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:59:38 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/13 15:09:06 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_is_export_only(t_ms *ms, char *key)
{
	t_list	*current;

	current = ms->export_only;
	while (current)
	{
		if (!ft_strncmp(current->content, key, ft_strlen(key)))
			return (1);
		current = current->next;
	}
	return (0);
}

// Add helper function to remove from export_only list
void	ms_remove_from_export_only(t_ms *ms, char *key)
{
	t_list	*current;
	t_list	*prev;

	current = ms->export_only;
	prev = NULL;
	while (current)
	{
		if (!ft_strncmp(current->content, key, ft_strlen(key)))
		{
			if (prev)
				prev->next = current->next;
			else
				ms->export_only = current->next;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/*
Helper to print a single environment variable in export format
*/
void	ms_print_env_var(t_ms *ms, char *env_var)
{
	char	*sym;
	char	*out;

	sym = ft_strchr(env_var, '=');
	if (sym)
		out = ms_build_export_output(ms, env_var, sym);
	else
	{
		out = ft_strdup(env_var);
		gc_add(out, &ms->gc);
	}
	ft_printf("declare -x %s\n", out);
}

/*
Helper to print export-only variables
*/
void	ms_print_export_only(t_list *export_var)
{
	while (export_var)
	{
		ft_printf("declare -x %s\n", (char *)export_var->content);
		export_var = export_var->next;
	}
}

/*
Main export print function that handles both environment and export-only vars
*/
int	ms_export_print(t_ms *ms, char **env)
{
	int	i;

	i = 0;
	env = ms_sort(env, ft_memcmp);
	while (env[i])
		ms_print_env_var(ms, env[i++]);
	ms_print_export_only(ms->export_only);
	return (0);
}
