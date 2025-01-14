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

/*
	Replaces a variable with its value from the env list if a match for the key
	is found in the env list.
*/
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
	new = (char *)malloc(sizeof(char) \
		* (ft_strlen(str) + ft_strlen(var) - ft_strlen(key)) + 3);
	if (!new)
		ms_error_handler(ms, "Error: Malloc failed expanding a variable", 1);
	new[++i] = '"';
	while (++i < mark)
		new[i] = str[i];
	while (var[++j])
		new[i + j] = var[j];
	k = ft_strlen(key) + i - 1;
	i += j;
	new[i++] = '"';
	new[i] = '\0';
	return (new);
}

/*
	Replaces any $VAR that is not set in env and/or is NULL by NULL within
	the original string or returns an empty string if the variable is the token.
*/
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

/*
	Replaces the variable '$?' with the exit status of the latest
	executed command within a string or as a single token.
*/
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

/*
	If the char '$' is found in the token, it gets the key and looks for
	the corresponding value in the env list. Only handles $? as a special
	case, $@ and other cases get replaced by NULL.
*/
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
		status = ft_itoa(ms->exit_status);
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

/*
	Iterates over the list of tokens in the structure ms and checks for '$'.
	If the token matches, the content of token is splitted into subtokens.
	Each subtoken is expanded if conditions are met, then are merged back
	into a single token.
*/
void	ms_expand_variable(t_ms *ms)
{
	t_token	*aux;
	t_token	*subtok;
	char	*tmp;

	aux = ms->chain_tokens;
	subtok = NULL;
	while (aux)
	{
		if (aux->type == 0 && ft_strchr(aux->content, '$'))
		{
			tmp = ft_strdup(aux->content);
			ms_process_token_content(ms, tmp, &subtok);
			free(tmp);
			ms_expand_subtoken(ms, subtok);
			gc_add(aux->content, &ms->gc);
			aux->content = ms_merge_subtoken(ms, subtok);
			ms_tokclear(&subtok, free);
		}
		aux = aux->next;
	}
}
