/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:53:16 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 15:49:47 by nponchon         ###   ########.fr       */
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

char	*ms_get_hostname(char *session_manager)
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
		gc_add(hostname);
		return (hostname);
	}
	else
		return (NULL);
}

char	*ms_get_prompt_user(t_list *ms_env)
{
	char	*username;
	char	*hostname;
	char	*prompt_user;
	char	*session_manager;

	hostname = NULL;
	username = ms_get_env_variable(ms_env, "USER=");
	if (!username)
		username = ms_username_from_psswd();
	if (!username)
		username = "unknown";
	session_manager = ms_get_env_variable(ms_env, "SESSION_MANAGER=");
	if (session_manager)
		hostname = ms_get_hostname(session_manager);
	if (!hostname)
		hostname = "localhost";
	prompt_user = ft_strjoin(username, "@");
	gc_add(prompt_user);
	prompt_user = ft_strjoin(prompt_user, hostname);
	gc_add(prompt_user);
	prompt_user = ft_strjoin(prompt_user, ":");
	gc_add(prompt_user);
	return (prompt_user);
}

char	*ms_get_cwd(t_list *ms_env)
{
	char	*cwd;
	int		aux;

	cwd = ms_get_env_variable(ms_env, "PWD=");
	if (!cwd)
	{
		cwd = malloc(PATH_MAX);
		if (!cwd)
			ms_error_handler("Error: Mem alloc failed for PWD", 1);
		gc_add(cwd);
		if (getcwd(cwd, PATH_MAX) == NULL)
		{
			ms_error_handler("Error: Unable to retrieve CWD", 0);
			cwd = "unknown_directory";
		}
	}
	aux = 3;
	while (aux--)
		cwd = ft_strchr(cwd, '/') + 1;
	cwd--;
	cwd = ft_strjoin("~", cwd);
	gc_add(cwd);
	return (cwd);
}

char	*ms_build_prompt(t_list *ms_env)
{
	char	*username;
	char	*cwd;
	char	*prompt;
	char	*tmp;

	username = ms_get_prompt_user(ms_env);
	gc_add(username);
	cwd = ms_get_cwd(ms_env);
	prompt = ft_strjoin("minishell> ", username);
	gc_add(prompt);
	if (!prompt)
		ms_error_handler("Error: Mem alloc failed for prompt", 1);
	tmp = ft_strjoin(prompt, " ");
	gc_add(tmp);
	prompt = ft_strjoin(tmp, cwd);
	gc_add(prompt);
	tmp = ft_strjoin(prompt, "$ ");
	return (tmp);
}
