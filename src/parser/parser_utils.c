/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 10:56:13 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/16 12:56:41 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_skip_squote(char *str, int *i)
{
	if (str[*i] == S_QUOTE)
	{
		(*i)++;
		while (str[*i] != S_QUOTE)
			(*i)++;
	}
	return ;
}

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

char	*ms_get_key(t_ms *ms, char *str)
{
	char	*key;
	int		i;

	i = 0;
	if (!str[i] || ft_isspace(str[i]) || is_quote(str[i]))
		key = ft_strdup("$");
	else
	{
		while (str[i] != '$' && str[i]
			&& !ft_isspace(str[i]) && !is_quote(str[i]))
			i++;
		key = ft_substr(str, 0, i);
	}
	gc_add(key, &ms->gc);
	printf("%s\n", key);
	return (key);
}
