/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 18:39:21 by hmunoz-g         ###   ########.fr       */
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

char	*get_prompt_user(void)
{
	char	*username;

	username = getenv("USER");
	if (!username)
		username = ms_username_from_psswd();
	return (username);
}

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

void	ms_add_env_variable(t_list **ms_env, const char *env_var)
{
	t_list	*new_node;

	new_node = ft_lstnew(ft_strdup(env_var));
	if (!new_node)
		ms_error_handler("Error: env copy failed during node creation", 0);
	ft_lstadd_back(ms_env, new_node);
}

t_list	*ms_copy_env(t_list *ms_env, char **env)
{
	char	*user_entry;
	int		i;

	if (!env || !*env)
	{
		user_entry = ms_create_user_entry();
		ms_add_env_variable(&ms_env, user_entry);
		free(user_entry);
	}
	else
	{
		i = 0;
		while (env[i])
		{
			ms_add_env_variable(&ms_env, env[i]);
			i++;
		}
	}
	return (ms_env);
}
