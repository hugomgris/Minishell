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

char	*ms_replace_expanded(t_ms *ms, char *str, char *key, int mark)
{
	char	*new;
	char	*var;
	int		i;
	int		j;
	int		k;

	i = -1;
	j = -1;
	var = ms_get_env_variable(ms, key);
	printf("str=%s, key=%s, var=%s\n", str, key, var);
	new = (char *)malloc(sizeof(char) \
		* (ft_strlen(str) + ft_strlen(var) - ft_strlen(key)) + 1);
	if (!new)
		ms_error_handler(ms, "Error: Malloc failed expanding a variable", 1);
	while (++i < mark)
		new[i] = str[i];
	while (var[++j])
		new[i + j] = var[j];
	k = ft_strlen(key) + i;
	i += j;
	while (str[++k])
		new[i++] = str[k];
	new[i] = '\0';
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
	while (*str)
	{
		new[i++] = *str;
		str++;
	}
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
	return (new);
}

char	*ms_search_env(t_ms *ms, char *str, int start)
{
	char	*key;
	char	*status;
	t_list	*aux;

	aux = ms->ms_env;
	if (aux == NULL)
		return (0);
	key = ms_get_key(ms, str + start + 1);
	if (*key == '?')
	{
		status = ft_itoa(ms_get_set(0, 0));
		gc_add(status, &ms->gc);
		return (ms_replace_exit_status(ms, str, status));
	}
	while (aux != NULL)
	{
		if (ms_key_checker(key, aux->content))
			return (ms_replace_expanded(ms, str, key, start));
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
			if (!ms_skip_squote(str, &i))
				break ;
			else if (str[i] == '$' && str[i + 1] != '\0' \
				&& !ft_isspace(str[i + 1]) && str[i + 1] != '$'
				&& !is_quote(str[i + 1]))
			{
				str = ms_search_env(ms, aux->content, i);
				gc_add(aux->content, &ms->gc);
				aux->content = str;
				i = -1;
			}
		}
		aux = aux->next;
	}
}
