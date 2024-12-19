/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:25:37 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/19 16:30:21 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
