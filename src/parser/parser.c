/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/11 17:14:31 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_remove_quotes(t_ms *ms)
{
	t_list	*aux;
	char	*tmp;

	aux = ms->tokens;
	while (aux)
	{
		if (!ft_strncmp((char *)aux->content, "\"", 1))
		{
			tmp = ft_strtrim(aux->content, "\"");
			if (!tmp)
				ms_error_handler(ms, "Error: Malloc failed removing quotes", 1);
			free(aux->content);
			aux->content = tmp;
		}
		else if (!ft_strncmp((char *)aux->content, "\'", 1))
		{
			tmp = ft_strtrim(aux->content, "\'");
			if (!tmp)
				ms_error_handler(ms, "Error: Malloc failed removing quotes", 1);
			free(aux->content);
			aux->content = tmp;
		}
		aux = aux->next;
	}
}

int	ms_parser(t_ms *ms, char *str)
{
	if (!ms_syntax_checker(ms, str))
		return (FALSE);
	if (!ms_tokenizer(ms, str))
		return (FALSE);
	ms_expand_variable(ms);
	ms_remove_quotes(ms);
	return (TRUE);
}
