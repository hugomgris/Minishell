/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:53:16 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/29 14:32:24 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to prompt build. Retrieves hostname if reachable.
*/
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

/*
Helper function to prompt build. Retrieves username.
If unset env, calls for other helpers to retrieve root user. 
*/
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
	prompt_user = ft_strjoin(prompt_user, "\033[0;37m:");
	gc_add(prompt_user);
	return (prompt_user);
}

/*
Helper function to prompt build. Gets the Current Working Director.
*/
char	*ms_get_cwd(t_list *ms_env)
{
	char	*cwd;

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
	ms_add_env_variable(ms_make_home(cwd));
	cwd = ms_short_dir(cwd);
	gc_add(cwd);
	return (cwd);
}

/*
Composite prompt builder.
Creates the minishell prompt by calling different helpers. 
*/
char	*ms_build_prompt(t_list *ms_env)
{
	char	*username;
	char	*cwd;
	char	*prompt;
	char	*tmp;

	username = ms_get_prompt_user(ms_env);
	gc_add(username);
	cwd = ms_get_cwd(ms_env);
	prompt = ft_strjoin("\033[1;41mminishell>\033[0m\033[1;32m ", username);
	gc_add(prompt);
	if (!prompt)
		ms_error_handler("Error: Mem alloc failed for prompt", 1);
	tmp = ft_strjoin(prompt, " ");
	gc_add(tmp);
	prompt = ft_strjoin(tmp, "\033[0m\033[1;34m");
	gc_add(prompt);
	tmp = ft_strjoin(prompt, cwd);
	gc_add(tmp);
	tmp = ft_strjoin(tmp, "\033[0m$ ");
	return (tmp);
}
