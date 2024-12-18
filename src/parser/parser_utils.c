/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 10:56:13 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/17 17:30:03 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Advances the pointer of a string when it finds an opening single quote,
	until it reaches the closing quote. The string NEEDS to be correctly quoted,
	otherwise SEGFAULT happens.
	If the closing single quote is the last character of the string,
	it returns NULL.
*/
int	ms_ignore_squote(char *str, int *i)
{
	int		j;
	char	quote;

	j = -1;
	quote = 0;
	while (str[++j] || j < *i)
	{
		if (str[j] == D_QUOTE)
			quote = D_QUOTE;
	}
	if (quote)
		return (1);
	if (str[*i] == S_QUOTE)
	{
		(*i)++;
		while (str[*i] != S_QUOTE)
			(*i)++;
	}
	if (*str == 0)
		return (0);
	return (1);
}

/*
	Extracts a token from a string that is within quotes (double or single).
	Currently obsolete, as the extraction is done with ms_extract_atom instead.
*/
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

/*
	Extracts a token from a string that is within quotes (double or single).
	Currently obsolete, as the extraction is done with ms_extract_atom.
*/
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

/*
	Gets the corresponding key after identifying a '$'in a string.
	If '$' is directly followed by a separator, returns '$'.
	Else, returns the key without the '$'. 
*/
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
	return (key);
}
