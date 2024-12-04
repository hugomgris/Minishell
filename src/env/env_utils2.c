/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/03 18:50:53 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
			gc_add(username, &ms->gc);
			break ;
		}
		line = get_next_line(fd);
	}
	close(fd);
	gc_add(line, &ms->gc);
	return (username);
}

char	*ms_get_prompt_user(t_ms *ms)
{
	char	*username;
	char	*hostname;
	char	*prompt_user;
	char	*session_manager;

	hostname = NULL;
	username = ms_get_username(ms);
	if (!username)
		username = "unknown";
	session_manager = ms_get_env_variable(ms, "SESSION_MANAGER=");
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

char	*ms_get_cwd(t_ms *ms)
{
	char	*cwd;
	int		aux;

	cwd = ms_get_env_variable(ms, "PWD=");
	aux = 3;
	while (aux--)
		cwd = ft_strchr(cwd, '/') + 1;
	cwd--;
	if (!cwd)
	{
		return ("~");
	}
	cwd = ft_strjoin("~", cwd);
	gc_add(cwd, &ms->gc);
	return (cwd);
}

char	*ms_get_username(t_ms *ms)
{
	char	*username;

	username = ms_get_env_variable(ms, "USER=");
	if (!username)
		username = ms_username_from_psswd(ms);
	if (!username)
		username = "unknown";
	return (username);
}
