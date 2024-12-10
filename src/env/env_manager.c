/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/10 15:49:12 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ms_make_home_ref(t_ms *ms, char **env)
{
	char	*cwd;
	int		i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "HOME", 4))
			return (env[i] + 5);
		i++;
	}
	cwd = NULL;
	cwd = getcwd(cwd, PATH_MAX);
	if (!cwd)
		return (NULL);
	gc_add(cwd, &ms->gc);
	cwd = ft_strchr(cwd, '/');
	gc_add(cwd, &ms->gc);
	cwd = ft_substr(cwd, 0, ft_strchr_n(cwd, '/', 3) - cwd);
	gc_add(cwd, &ms->gc);
	return (cwd);
}

void	ms_set_env_variable(t_ms *ms, char *key, char *value)
{
	t_list	*env;
	char	*entry;
	size_t	key_len;

	key_len = ft_strlen(key);
	env = ms->ms_env;
	while (env)
	{
		if (!ft_strncmp((char *)env->content, key, key_len)
			&& ((char *)env->content)[key_len] == '=')
		{
			free(env->content);
			entry = ft_strjoin(key, "=");
			gc_add(entry, &ms->gc);
			entry = ft_strjoin(entry, value);
			env->content = entry;
			return ;
		}
		env = env->next;
	}
	entry = ft_strjoin(key, "=");
	entry = ft_strjoin_free(entry, value);
	ft_lstadd_back(&ms->ms_env, ft_lstnew(entry));
}

void	ms_add_env_variable(t_ms *ms, const char *env_var)
{
	t_list	*new_node;

	new_node = ft_lstnew(ft_strdup(env_var));
	if (!new_node)
		ms_error_handler(ms, "Error: env copy failed", 0);
	ft_lstadd_back(&ms->ms_env, new_node);
}

char	*ms_get_env_variable(t_ms *ms, const char *var_name)
{
	t_list	*current;
	char	*value;

	current = ms->ms_env;
	while (current)
	{
		value = current->content;
		if (ft_strncmp(value, var_name, ft_strlen(var_name)) == 0)
			return (value + ft_strlen(var_name));
		current = current->next;
	}
	return (NULL);
}

t_list	*ms_copy_env(t_ms *ms, char **env)
{
	int		i;

	if (!env || !*env)
		return (NULL);
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
