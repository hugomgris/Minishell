/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:35:12 by nponchon          #+#    #+#             */
/*   Updated: 2024/11/28 20:01:41 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	Current state of things: the function looks up any $VAR string from
	the input in the envp. In case of exact match, the input is rewritten
	with expanded var, 	or with NULL if not found.
	!This does not take possible quotes or exceptions ($? etc) into account yet.
	*/

char	*ms_replace_expanded(char *str, char *key, char *var)
{
	char	*new;
	int		i;
	int		j;
	int		k;

	i = -1;
	j = -1;
	var += ft_strlen(key) + 1;
	new = (char *)malloc(sizeof(char) * (ft_strlen(str) \
		+ ft_strlen(var) - ft_strlen(key)) + 1);
	if (!new)
		return (NULL);
	while (str[++i] != '$')
		new[i] = str[i];
	while (var[++j])
		new[i + j] = var[j];
	k = ft_strlen(key) + i;
	while (str[++k])
		new[i + j++] = str[k];
	new[i + j] = '\0';
	return (new);
}

char	*ms_replace_null_value(char *str, char *key)
{
	char	*new;
	int		i;

	i = -1;
	new = (char *)malloc(sizeof(char) * (ft_strlen(str) \
		- ft_strlen(key)) + 2);
	if (!new)
		return (NULL);
	while (str[++i] != '$')
		new[i] = str[i];
	str += ft_strlen(key) + 1;
	while (str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

int	ms_key_checker(char *key, const char *var)
{
	int	i;

	i = -1;
	while (key[++i])
	{
		if (key[i] != var[i])
			return (0);
		if (key[i + 1] == '\0' && var[i + 1] == '=')
			return (1);
	}
	return (0);
}

char	*ms_search_env(t_list *ms_env, char *str, int start)
{
	char	*key;
	char	*tmp;

	tmp = ft_strdup(str);
	if (ms_env == NULL)
		return (0);
	key = strtok(tmp + start + 1, " ");
	while (ms_env != NULL)
	{
		if (ms_key_checker(key, ms_env->content))
		{
			str = ms_replace_expanded(str, key, ms_env->content);
			return (str);
		}
		ms_env = ms_env->next;
	}
	str = ms_replace_null_value(str, key);
	return (str);
}

char	*ms_expand_variable(t_list *ms_env, char *str)
{
	int		i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '$' && ft_isalnum(str[i + 1]))
			str = ms_search_env(ms_env, str, i);
	}
	return (str);
}
