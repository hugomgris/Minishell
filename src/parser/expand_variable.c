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

char	*ms_replace_expanded(t_ms *ms, char *str, char *key, char *var)
{
	char	*new;
	int		i;
	int		j;
	int		k;

	i = -1;
	j = -1;
	var += ft_strlen(key) + 1;
	new = (char *)malloc(sizeof(char) \
		* (ft_strlen(str) + ft_strlen(var) - ft_strlen(key)) + 1);
	if (!new)
		ms_error_handler(ms, "Error: Malloc failed expanding a variable", 1);
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

char	*ms_replace_null_value(t_ms *ms, char *str, char *key)
{
	char	*new;
	int		i;

	i = -1;
	if (ft_strlen(str) == ft_strlen(key))
		return (ft_strdup(""));
	new = (char *)malloc(sizeof(char) \
		* (ft_strlen(str) - ft_strlen(key)) + 2);
	if (!new)
		ms_error_handler(ms, "Error: Malloc failed expanding a variable", 1);
	while (str[++i] != '$')
		new[i] = str[i];
	str += i + ft_strlen(key) + 1;
	new[i++] = *str;
	new[i] = '\0';
	return (new);
}

char	*ms_replace_exit_status(t_ms *ms, char *str, char *status)
{
	char	*new;
	int		i;
	int		j;

	i = -1;
	j = -1;
	new = (char *)malloc(sizeof(char) \
		* ((ft_strlen(str) - 2 + ft_strlen(status)) + 1));
	if (!new)
		ms_error_handler(ms, "Error: Malloc failed expanding a variable", 1);
	while (str[++i] != '$')
		new[i] = str[i];
	while (status[++j])
		new[i + j] = status[j];
	str += 2;
	while (str[i])
	{
		new[i + j] = str[i];
		i++;
	}
	new[i + j] = '\0';
	free(status);
	return (new);
}

char	*ms_search_env(t_ms *ms, char *str, int start)
{
	char	*key;
	char	*tmp;
	char	*status;
	t_list	*aux;

	aux = ms->ms_env;
	tmp = ft_strdup(str);
	gc_add(tmp, &ms->gc);
	if (aux == NULL || tmp == NULL)
		return (0);
	key = ft_strtok(tmp + start + 1, " ^*$\"\'=/-+.:");
	if (*key == '?')
	{
		status = ft_itoa(ms->exit_status);
		return (ms_replace_exit_status(ms, str, status));
	}
	while (aux != NULL)
	{
		if (ms_key_checker(key, aux->content))
			return (ms_replace_expanded(ms, str, key, aux->content));
		aux = aux->next;
	}
	return (ms_replace_null_value(ms, str, key));
}

void	ms_expand_variable(t_ms *ms)
{
	t_list	*aux;
	char	*str;
	int		i;

	aux = ms->tokens;
	while (aux)
	{
		str = (char *)aux->content;
		i = -1;
		while (str[++i])
		{
			if (!str[i] || (!i && str[i] == 39))
				break ;
			if (str[i] == '$' && (str[i + 1] == '\0' \
				|| !ft_isspace(str[i + 1])))
			{
				str = ms_search_env(ms, aux->content, i);
				gc_add(aux->content, &ms->gc);
				aux->content = str;
				i = -1;
				continue ;
			}
		}
		aux = aux->next;
	}
}
