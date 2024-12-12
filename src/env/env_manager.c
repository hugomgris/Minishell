/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/12 10:47:07 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to build the ms_env entry that will be get/set/added.
It handles special cases with no value (used for export builtin).
*/
char	*ms_build_entry(char *key, char *value)
{
	char	*entry;
	char	*tmp;

	if (!value)
		return (ft_strdup(key));
	tmp = ft_strjoin(key, "=");
	entry = ft_strjoin(tmp, value);
	free(tmp);
	return (entry);
}

/*
Changes the value of an existing env variable.
*/
void	ms_set_env_variable(t_ms *ms, char *key, char *value)
{
	t_list	*env;
	char	*entry;

	env = ms->ms_env;
	entry = ms_build_entry(key, value);
	while (env)
	{
		if (!ft_strncmp(env->content, key, ft_strlen(key))
			/*&& ((char *)env->content)[ft_strlen(key)] == '='*/)
		{
			free(env->content);
			env->content = entry;
			return ;
		}
		env = env->next;
	}
	ft_lstadd_back(&ms->ms_env, ft_lstnew(entry));
}

/*
Adds an env variable to the ms_env list.
Recieves the whole var as a string ("KEY=VAL").
The build of the var is handled by calling function.
*/
void	ms_add_env_variable(t_ms *ms, char *key, char *value)
{
	t_list	*new_node;
	char	*entry;

	entry = ms_build_entry(key, value);
	new_node = ft_lstnew(entry);
	if (!new_node)
		ms_error_handler(ms, "Error: env copy failed", 0);
	ft_lstadd_back(&ms->ms_env, new_node);
}

/*
Gets an environment variable from the ms_env list.
Gets the key ("NAME=") as argument to search through the entries.
If found, returns the value of the variable (i.e. what's after '=').
*/
char	*ms_get_env_variable(t_ms *ms, char *key)
{
	t_list	*current;
	size_t	key_len;
	char	*entry;

	current = ms->ms_env;
	key_len = ft_strlen(key);
	while (current)
	{
		entry = current->content;
		if (!ft_strncmp(entry, key, key_len) && entry[key_len] == '=')
			return (entry + key_len + 1);
		current = current->next;
	}
	return (NULL);
}

/*
Function to make a linked list copy of the env variables.
If unset env, returns NULL.
List is stored in the ms_env t_list inside the ms minishell struct.
*/
t_list	*ms_copy_env(t_ms *ms, char **env)
{
	int		i;
	char	**key_value;

	if (!env || !*env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		key_value = ft_split(env[i], '=');
		if (key_value)
		{
			ms_add_env_variable(ms, key_value[0], key_value[1]);
			ft_free(key_value);
		}
		i++;
	}
	return (ms->ms_env);
}
