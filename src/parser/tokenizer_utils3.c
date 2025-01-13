/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:17:24 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/13 14:26:51 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_check_multipleredir(t_ms *ms)
{
	t_token	*aux;

	aux = ms->tok;
	while (aux && aux->next)
	{
		if (aux->type == T_DBLESS && aux->next->type != T_ATOM)
		{
			ms_error_handler(ms, "invalid syntax with heredoc", 0);
			return (FALSE);
		}
		else if (ms_is_operator(aux->content) && aux->next->type != T_ATOM)
		{
			ms_error_handler(ms, "invalid syntax with redirections", 0);
			return (FALSE);
		}
		aux = aux->next;
	}
	return (TRUE);
}

/*
	Does various checks of the newly formed tokens, like parenthesis syntax,
	correct redirections, etc.
*/
int	ms_check_tokens(t_ms *ms)
{
	if (!ms_check_redirparenthesis(ms))
		return (FALSE);
	if (!ms_check_multipleredir(ms))
		return (FALSE);
	return (TRUE);
}
