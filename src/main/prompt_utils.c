/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:53:16 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 13:00:46 by hmunoz-g         ###   ########.fr       */
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
	prompt_user = ft_strjoin_free(prompt_user, hostname);
	prompt_user = ft_strjoin_free(prompt_user, ":");
	if (ft_strncmp(hostname, "localhost", 9))
		free(hostname);
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
		if (getcwd(cwd, PATH_MAX) == NULL)
		{
			free(cwd);
			ms_error_handler("Error: Unable to retrieve CWD", 0);
			cwd = "unknown_directory";
		}
	}
	aux = 3;
	while (aux--)
		cwd = ft_strchr(cwd, '/') + 1;
	cwd--;
	cwd = ft_strjoin("~", cwd);
	return (cwd);
}

char	*ms_build_prompt(t_list *ms_env)
{
	char	*username;
	char	*cwd;
	char	*prompt;
	char	*tmp;

	username = ms_get_prompt_user(ms_env);
	cwd = ms_get_cwd(ms_env);
	prompt = ft_strjoin("minishell> ", username);
	if (!prompt)
		ms_error_handler("Error: Mem alloc failed for prompt", 1);
	tmp = ft_strjoin(prompt, " ");
	free(prompt);
	prompt = ft_strjoin(tmp, cwd);
	free(tmp);
	tmp = ft_strjoin(prompt, "$ ");
	free(prompt);
	free(username);
	return (tmp);
}
