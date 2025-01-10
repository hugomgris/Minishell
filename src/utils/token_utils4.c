/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:09:10 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/10 18:09:27 by nponchon         ###   ########.fr       */
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
		res = ft_strjoin_free(res, (char *)aux->content);
		if (!res)
			ms_error_handler(ms, "Malloc failed expanding a variable", 1);
		aux = aux->next;
	}
	return (res);
}
