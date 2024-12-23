/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:20:34 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/21 12:31:22 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_export_print(t_ms *ms, char **env)
{
	char	*sym;
	char	*out;
	int		i;

	i = 0;
	while (env[i])
	{
		sym = ft_strchr(env[i], '=');
		if (sym)
			out = ms_build_export_output(ms, env[i], sym);
		else
		{
			out = ft_strdup(env[i]);
			gc_add(out, &ms->gc);
		}
		ft_printf("declare -x %s\n", out);
		i++;
	}
	return (0);
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

int	ms_export_ex(t_ms *ms, char *key, char *value)
{
	if (!ms_export_check(key))
		return (ms_export_error(ms, key));
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
	return (0);
}

int	ms_export(t_ms *ms, char **cmd_args, char **env)
{
	int		i;
	char	*key;
	char	*value;

	if (!cmd_args[1])
		return (ms_export_print(ms, env));
	i = 0;
	while (cmd_args[++i])
	{
		if (ms_export_check(cmd_args[i]))
		{
			key = cmd_args[i];
			value = ft_strchr(key, '=');
			if (value)
			{
				*value = '\0';
				if (ms_export_ex(ms, key, value + 1) != 0)
					return (1);
			}
			else
			{
				if (ms_export_ex(ms, key, NULL) != 0)
					return (1);
			}
		}
		else
			return (ms_export_error(ms, cmd_args[i]));
	}
	return (0);
}
