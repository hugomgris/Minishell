/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:49:40 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/04 17:51:02 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_operator(char *c)
{
	if (*c == '<' || *c == '>' || *c == '|' || *c == '&'
		|| *c == '(' || *c == ')' || *c == '\t')
		return (TRUE);
	return (FALSE);
}

void	ms_skip_space(char **str)
{
	while (**str && ft_isspace(**str))
		(*str)++;
}

void	ms_skip_quotes(char	*str, int *i)
{
	char	quote;

	quote = str[*i];
	(*i)++;
	while (str[*i] != quote)
	{
		(*i)++;
	}
	(*i)++;
}
