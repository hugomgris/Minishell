/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/09 11:38:51 by hmunoz-g         ###   ########.fr       */
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

	aux = ms->tok;
	while (aux)
	{
		tmp = ft_strdup((char *)aux->content);
		gc_add(tmp, &ms->gc);
		if (ft_strchr(tmp, S_QUOTE) || ft_strchr(tmp, D_QUOTE))
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

/*void	ms_copy_toktolist(t_ms *ms)
{
	t_token	*aux;
	t_list	*new;

	new = NULL;
	aux = ms->tok;
	while (aux)
	{
		new = ft_lstnew(ft_strdup((char *)aux->content));
		ft_lstadd_back(&ms->tokens, new);
		aux = aux->next;
	}
}*/

/*
	The control tower for the parsing process.
	Does a preliminary check of the user input and yields error
	in case of incorrect input. Input get then tokenised, and the tokens
	containing variable are expanded to their correct values. 
	Empty tokens are then removed and the unnecessary quotes are trimmed.
*/
int	ms_parser(t_ms *ms, char *str)
{
	if (!ms_syntax_checker(ms, str))
		return (FALSE);
	if (!ms_tokenizer(ms, str))
		return (FALSE);
	ms_expand_variable(ms);
	ms_expand_wildcards(ms);
	ms_remove_quotes(ms);
	ms_remove_empty_tokens(&ms->tok, free);
	ms_tokclear(&ms->wc, free);
	return (TRUE);
}
