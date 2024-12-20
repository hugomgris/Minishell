/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:25:37 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/20 12:05:33 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_expand_subtoken(t_ms *ms, t_token *lst)
{
	t_token	*aux;
	char	*str;
	int		i;

	aux = lst;
	while (aux)
	{
		str = (char *)aux->content;
		i = -1;
		while (str[++i])
		{
			if (str[i] == S_QUOTE && i == 0)
				break ;
			else if (str[i] == '$' && str[i + 1] != '\0' \
				&& str[i + 1] != '$')
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

char	*ms_merge_subtoken(t_ms *ms, t_token *subtok)
{
	t_token	*aux;
	char	*res;

	aux = subtok;
	res = ft_strdup("");
	if (!res)
		ms_error_handler(ms, "Malloc failed expanding a variable", 1);
	while (aux)
	{
		res = ft_strjoin(res, (char *)aux->content);
		if (!res)
			ms_error_handler(ms, "Malloc failed expanding a variable", 1);
		aux = aux->next;
	}
	return (res);
}

void	process_quotes(char **tmp, t_token **subtok, char quote)
{
	int	i;

	i = 1;
	while ((*tmp)[i] && (*tmp)[i] != quote)
		i++;
	if ((*tmp)[i] == quote)
		i++;
	ms_tokadd_back(subtok, ms_new_token(ft_substr(*tmp, 0, i), T_ATOM));
	*tmp += i;
}

void	process_unquoted(char **tmp, t_token **subtok)
{
	int	i;

	i = 0;
	while ((*tmp)[i] && !is_quote((*tmp)[i]))
		i++;
	ms_tokadd_back(subtok, ms_new_token(ft_substr(*tmp, 0, i), T_ATOM));
	*tmp += i;
}

void	process_token_content(char *tmp, t_token **subtok)
{
	while (*tmp)
	{
		if (*tmp == D_QUOTE || *tmp == S_QUOTE)
			process_quotes(&tmp, subtok, *tmp);
		else
			process_unquoted(&tmp, subtok);
	}
}
