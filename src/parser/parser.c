/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/14 09:58:56 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Counts the number of quotes to remove for reallocation of the shorter string.
*/
int	ms_count_quotes(char *str)
{
	int		count;
	char	quote;

	count = 0;
	quote = 0;
	while (*str)
	{
		if (!quote && (*str == S_QUOTE || *str == D_QUOTE))
		{
			quote = *str;
			count++;
		}
		else if (*str == quote)
		{
			quote = 0;
			count++;
		}
		str++;
	}
	return (count);
}

/*
	Removes unnecessary quotes after the variable expansion has occured.
*/
char	*ms_trim_quotes(char *str, char *new, int len)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (i < len)
	{
		if (!quote && (str[i] == S_QUOTE || str[i] == D_QUOTE))
		{
			quote = str[i];
			str++;
		}
		else if (str[i] == quote)
		{
			quote = 0;
			str++;
		}
		else
		{
			new[i] = str[i];
			i++;
		}
	}
	new[len] = 0;
	return (new);
}

/*
	Iterates over the content of a token and removes the unnecessary quotes.
*/
void	ms_remove_quotes(t_ms *ms)
{
	t_token	*aux;
	char	*tmp;
	char	*new;
	int		count;
	int		len;

	aux = ms->chain_tokens;
	while (aux)
	{
		tmp = ft_strdup(aux->content);
		gc_add(tmp, &ms->gc);
		if ((ft_strchr(tmp, S_QUOTE) || ft_strchr(tmp, D_QUOTE)) \
			&& aux->type == 0)
		{
			count = ms_count_quotes(tmp);
			len = ft_strlen(tmp) - count;
			new = (char *)malloc(sizeof(char) * len + 1);
			if (!new)
				ms_exit_handler(ms, "Error: Malloc failed trimming a quote", 1);
			gc_add(aux->content, &ms->gc);
			aux->content = ms_trim_quotes(tmp, new, len);
		}
		aux = aux->next;
	}
}

/*
	The control tower for the parsing process.
	Does a serie of preliminary checks of user input and yields error
	in case of incorrect syntax. Input get then tokenised, and the tokens
	containing variable are expanded to their correct values. 
	Unnecessary quotes are trimmed and resulting empty tokens are deleted.
*/
int	ms_parser(t_ms *ms)
{
	ms_expand_variable(ms);
	ms_expand_wildcards(ms);
	ms_remove_quotes(ms);
	ms_remove_empty_tokens(&ms->chain_tokens, free);
	ms_tokclear(&ms->wc, free);
	if (!ms->chain_tokens)
		return (FALSE);
	return (TRUE);
}
