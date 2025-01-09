/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/09 08:57:57 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Prompt helper function used as fallback if username retrieval fails.
Goes to the /etc/passwd file and extracts "root" value from its content.
*/
char	*ms_username_from_psswd(t_ms *ms)
{
	int		fd;
	char	*line;
	char	*username;
	char	*token;

	line = NULL;
	username = "unknown";
	fd = open("/etc/passwd", O_RDONLY);
	if (fd == -1)
		return (username);
	line = get_next_line(fd);
	while (line)
	{
		token = ft_strtok(line, ":");
		if (token)
		{
			username = ft_strdup(token);
			close(fd);
			free(line);
			gc_add(username, &ms->gc);
			break ;
		}
		line = get_next_line(fd);
	}
	return (username);
}

/*
Prompt helper function to build the username+hostname chunk of the prompt.
Calls username and hostname functions, concatenates return values.
*/
char	*ms_get_prompt_user(t_ms *ms)
{
	char	*username;
	char	*hostname;
	char	*prompt_user;
	char	*session_manager;

	if (ms->user)
		return (ms->user);
	hostname = NULL;
	username = ms_get_username(ms);
	if (!username)
		username = "user";
	session_manager = ms_get_env_variable(ms, "SESSION_MANAGER");
	if (session_manager)
		hostname = ms_get_hostname(session_manager, ms);
	if (!hostname)
		hostname = "localhost";
	prompt_user = ft_strjoin(username, "@");
	gc_add(prompt_user, &ms->gc);
	prompt_user = ft_strjoin(prompt_user, hostname);
	gc_add(prompt_user, &ms->gc);
	prompt_user = ft_strjoin(prompt_user, "\033[0;37m:");
	gc_add(prompt_user, &ms->gc);
	return (prompt_user);
}

/*
Prompt helper function to get the hostname from the session_manager.
If retrieval fails, falls back to a default "localhost" value.
*/
char	*ms_get_hostname(char *session_manager, t_ms *ms)
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
		gc_add(hostname, &ms->gc);
		return (hostname);
	}
	else
	{
		hostname = ft_strdup("localhost");
		gc_add(hostname, &ms->gc);
		return (hostname);
	}
}

/*
Prompt helper function, also useful for anything that needs the CWD. 
If ms_env has a PWD variable, returns it's value.
Else, tries to getcwd().
Else, falls back to an unknown value ("?"), as Bash does with a "."
	This is needed for an unset env, non-existing dir minishell execution.
*/
char	*ms_get_cwd(t_ms *ms)
{
	char	*cwd;
	char	*new_cwd;

	if (ms_get_env_variable(ms, "PWD"))
		cwd = ft_strdup(ms_get_env_variable(ms, "PWD"));
	else
	{
		cwd = NULL;
		cwd = getcwd(cwd, PATH_MAX);
	}
	if (!cwd)
		cwd = ft_strdup("?");
	gc_add(cwd, &ms->gc);
	if (cwd[0] == '/' && !cwd[1])
		return ("/");
	if (ms->home && ft_strncmp(cwd, ms->home, ft_strlen(ms->home)) == 0)
	{
		new_cwd = ft_strjoin("~", cwd + ft_strlen(ms->home));
		if (!new_cwd)
			ms_error_handler(ms, "Memory allocation failed", 1);
		gc_add(new_cwd, &ms->gc);
		return (new_cwd);
	}
	return (cwd);
}

/*
Prompt helper function.
Gets the USER value from ms_env, or calls alternative methods.
*/
char	*ms_get_username(t_ms *ms)
{
	char	*username;

	username = ms_get_env_variable(ms, "USER");
	if (!username)
		username = ms_username_from_psswd(ms);
	if (!username)
		username = "user";
	return (username);
}
