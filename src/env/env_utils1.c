/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 17:55:09 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/03 17:56:08 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
			entry = ft_strjoin_free(entry, value);
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
