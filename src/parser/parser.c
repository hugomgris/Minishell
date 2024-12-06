/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/05 19:29:24 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_parser(t_ms *ms, char *str)
{
	if (!ms_syntax_checker(ms, str))
		return (FALSE);
	if (!ms_tokenizer(ms, str))
		return (FALSE);
	ms_expand_variable(ms);
	return (TRUE);
}
