/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/29 14:39:56 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Recover username in case of unset env.
Fetches and reads /etc/psswd, tokenizes user (root), and returns it.
*/
char	*ms_username_from_psswd(void)
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
			break ;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	gc_add(line);
	return (username);
}

/*
Helper function to prompt build. Gets user from USER= entry.
If USER= doesn't exist, tryes to make it from /etc/passwd (root).
*/
char	*get_prompt_user(void)
{
	char	*username;

	username = getenv("USER=");
	if (!username)
		username = ms_username_from_psswd();
	return (username);
}

/*
Helper function to prompt build. Creates the complete entry for user.
	This means it gets the user via other helpers, adds "USER=".
*/
char	*ms_create_user_entry(void)
{
	char	*username;
	char	*user_entry;

	username = get_prompt_user();
	if (!username)
	{
		username = ft_strdup("unknown");
		if (!username)
		{
			ms_error_handler("Error: Mem alloc failed for def username", 1);
			return (NULL);
		}
	}
	user_entry = ft_strjoin("USER=", username);
	free(username);
	if (!user_entry)
		ms_error_handler("Error: Memory allocation failed", 1);
	return (user_entry);
}

/*
Makes a linked list copy of env variables.
If env is unset, creates a "USER=" entry with helpers.
*/
t_list	*ms_copy_env(char **env)
{
	char	*user_entry;
	int		i;
	t_list	**ms_env;

	ms_env = ms_env_instance();
	if (!env || !*env)
	{
		user_entry = ms_create_user_entry();
		ms_add_env_variable(user_entry);
		free(user_entry);
	}
	else
	{
		i = 0;
		while (env[i])
		{
			ms_add_env_variable(env[i]);
			i++;
		}
	}
	return (*ms_env);
}
