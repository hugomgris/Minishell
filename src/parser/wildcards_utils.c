/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:06:53 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/08 11:54:04 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_is_hidden_entry(char *entry)
{
	if (entry[0] == '.')
		return (TRUE);
	return (FALSE);
}

void	ms_handle_star(t_match_data *data, int *start)
{
	*start = data->j;
	data->start = *start;
	data->match = data->i;
	data->j++;
	return ;
}

int	ms_retry_star(t_match_data *data, int *start)
{
	if (*start == -1)
		return (FALSE);
	data->j = *start + 1;
	data->match++;
	data->i = data->match;
	return (TRUE);
}

void	ms_init_match_data(t_match_data *data)
{
	data->i = 0;
	data->j = 0;
	data->start = -1;
	data->match = 0;
	return ;
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
		if (ft_strcmp(tok->content, tok->next->content) > 0)
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
