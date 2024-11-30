/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/30 16:12:16 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Recover username in case of unset env.
Fetches and reads /etc/psswd, tokenizes user (root), and returns it.
*/
char	*ms_username_from_psswd(t_list **gc)
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
			gc_add(username, gc);
			break ;
		}
		line = get_next_line(fd);
	}
	close(fd);
	gc_add(line, gc);
	return (username);
}

char	*ms_get_prompt_user(t_list **ms_env, t_list **gc)
{
	char	*username;
	char	*hostname;
	char	*prompt_user;
	char	*session_manager;

	hostname = NULL;
	username = ms_get_env_variable(ms_env, "USER=");
	if (!username)
		username = ms_username_from_psswd(gc);
	if (!username)
		username = "unknown";
	session_manager = ms_get_env_variable(ms_env, "SESSION_MANAGER=");
	if (session_manager)
		hostname = ms_get_hostname(session_manager, gc);
	if (!hostname)
		hostname = "localhost";
	prompt_user = ft_strjoin(username, "@");
	gc_add(prompt_user, gc);
	prompt_user = ft_strjoin(prompt_user, hostname);
	gc_add(prompt_user, gc);
	prompt_user = ft_strjoin(prompt_user, "\033[0;37m:");
	gc_add(prompt_user, gc);
	return (prompt_user);
}

//!mirar si hace falta gc_add la linea 85.
char	*ms_create_user_entry(t_list **ms_env, t_list **gc)
{
	char	*username;
	char	*user_entry;

	username = ms_get_prompt_user(ms_env, gc);
	if (!username)
	{
		username = ft_strdup("unknown");
		gc_add(username, gc);
		if (!username)
		{
			ms_error_handler(ms_env, gc, "Error: Memory allocation failed", 1);
			return (NULL);
		}
	}
	user_entry = ft_strjoin("USER=", username);
	if (!user_entry)
		ms_error_handler(ms_env, gc, "Error: Memory allocation failed", 1);
	gc_add(user_entry, gc);
	return (user_entry);
}

void	ms_add_env_variable(t_list **ms_env, const char *env_var, t_list **gc)
{
	t_list	*new_node;

	new_node = ft_lstnew(ft_strdup(env_var));
	if (!new_node)
		ms_error_handler(ms_env, gc, "Error: env copy failed", 0);
	ft_lstadd_back(ms_env, new_node);
}

t_list	*ms_copy_env(t_list **ms_env, char **env, t_list **gc)
{
	char	*user_entry;
	int		i;

	if (!env || !*env)
	{
		user_entry = ms_create_user_entry(ms_env, gc);
		ms_add_env_variable(ms_env, user_entry, gc);
		gc_add(user_entry, gc);
	}
	else
	{
		i = 0;
		while (env[i])
		{
			ms_add_env_variable(ms_env, env[i], gc);
			i++;
		}
	}
	return (*ms_env);
}
