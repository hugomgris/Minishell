/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/03 18:46:51 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Recover username in case of unset env.
Fetches and reads /etc/psswd, tokenizes user (root), and returns it.
*/

char	*ms_create_user_entry(t_ms *ms)
{
	char	*username;
	char	*user_entry;

	username = ms_get_username(ms);
	if (!username)
	{
		username = ft_strdup("unknown");
		gc_add(username, &ms->gc);
		if (!username)
		{
			ms_error_handler(ms, "Error: Memory allocation failed", 1);
			return (NULL);
		}
	}
	user_entry = ft_strjoin("USER=", username);
	if (!user_entry)
		ms_error_handler(ms, "Error: Memory allocation failed", 1);
	gc_add(user_entry, &ms->gc);
	return (user_entry);
}

char	*ms_create_pwd_entry(t_ms *ms, char *cwd)
{
	cwd = malloc(PATH_MAX);
	if (!cwd)
		ms_error_handler(ms, "Error: Mem alloc failed for PWD", 1);
	gc_add(cwd, &ms->gc);
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ms_error_handler(ms, "Error: Unable to retrieve CWD", 0);
		cwd = "unknown_directory";
	}
	ms_set_env_variable(ms, "PWD", cwd);
	ms_set_env_variable(ms, "OLDPWD", cwd);
	return (cwd);
}

t_list	*ms_copy_env(t_ms *ms, char **env)
{
	char	*user_entry;
	char	*cwd;
	int		i;

	if (!env || !*env)
	{
		user_entry = ms_create_user_entry(ms);
		ms_add_env_variable(ms, user_entry);
		cwd = NULL;
		cwd = ms_create_pwd_entry(ms, cwd);
	}
	else
	{
		i = 0;
		while (env[i])
		{
			ms_add_env_variable(ms, env[i]);
			i++;
		}
	}
	return (ms->ms_env);
}
