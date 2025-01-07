/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:06:53 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/07 15:44:39 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_compare_wildcards(t_token *first, t_token *second)
{
	return (ms_tokcmp(first->content, second->content));
}

t_token	*ms_tokensort(t_token *tok)
{
	char	*aux;
	t_token	*start;

	if (!tok || !tok->next)
		return (tok);
	start = tok;
	while (tok && tok->next)
	{
		if (ms_compare_wildcards(tok, tok->next) > 0)
		{
			aux = (char *)tok->content;
			tok->content = tok->next->content;
			tok->next->content = aux;
			tok = start;
		}
		else
			tok = tok->next;
	}
	return (start);
}
