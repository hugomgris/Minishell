/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/30 17:07:24 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ms_get_env_variable(t_list *ms_env, const char *var_name)
{
	t_list	*current;
	char	*value;

	current = ms_env;
	while (current)
	{
		value = current->content;
		if (ft_strncmp(value, var_name, ft_strlen(var_name)) == 0)
			return (value + ft_strlen(var_name));
		current = current->next;
	}
	return (NULL);
}

char	*ms_get_hostname(char *session_manager, t_list **gc)
{
	char	*start;
	char	*end;
	char	*hostname;

	start = ft_strchr(session_manager, '/');
	if (start)
	{
		start++;
		end = ft_strchr(start, '.');
		if (end)
			hostname = ft_substr(start, 0, end - start);
		else
			hostname = ft_strdup(start);
		gc_add(hostname, gc);
		return (hostname);
	}
	else
		return (NULL);
}

char	*ms_get_cwd(t_list **ms_env, t_list **gc)
{
	char	*cwd;
	int		aux;

	cwd = ms_get_env_variable(*ms_env, "PWD=");
	if (!cwd)
	{
		cwd = malloc(PATH_MAX);
		if (!cwd)
			ms_error_handler(ms_env, gc, "Error: Mem alloc failed for PWD", 1);
		gc_add(cwd, gc);
		if (getcwd(cwd, PATH_MAX) == NULL)
		{
			ms_error_handler(ms_env, gc, "Error: Unable to retrieve CWD", 0);
			cwd = "unknown_directory";
		}
	}
	aux = 3;
	while (aux--)
		cwd = ft_strchr(cwd, '/') + 1;
	cwd--;
	cwd = ft_strjoin("~", cwd);
	gc_add(cwd, gc);
	return (cwd);
}
