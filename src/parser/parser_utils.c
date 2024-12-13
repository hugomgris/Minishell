/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 10:56:13 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/12 17:18:47 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_key_checker(char *key, const char *var)
{
	int	i;

	i = -1;
	while (key[++i])
	{
		if (key[i] != var[i])
			return (FALSE);
		if (key[i + 1] == '\0' && var[i + 1] == '=')
			return (TRUE);
	}
	return (FALSE);
}

int	ms_extract_quote(t_ms *ms, char **str)
{
	t_list	*node;
	char	quote;
	char	*token;
	int		i;

	i = 1;
	quote = **str;
	if (ft_strchr(*str + 1, quote))
	{
		token = ft_strdup(*str);
		if (!token)
			ms_error_handler(ms, "Error: Malloc failed allocating a token", 1);
		while (token[i] != quote)
			i++;
		free(token);
		token = ft_substr(*str, 0, ++i);
		node = ft_lstnew(token);
		if (!token || !node)
			ms_error_handler(ms, "Error: Malloc failed allocating a token", 1);
		ft_lstadd_back(&ms->tokens, node);
		*str += i;
		return (FALSE);
	}
	ms_error_handler(ms, "Error: Invalid quote", 0);
	return (TRUE);
}
