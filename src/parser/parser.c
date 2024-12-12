/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/12 22:07:55 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	TODO
	- count number of quotes to remove
	- realloc shorter string
	- copy new from old skipping unnecessary quotes
	- free old string
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

char	*ms_trim_quotes(t_ms *ms, char *str, int count)
{
	char	quote;
	char	*new;
	int		len;
	int		i;

	len = ft_strlen(str) - count;
	new = (char *)malloc(sizeof(char) * len + 1);
	if (!new)
		ms_exit_handler(ms, "Error: Malloc failed trimming a quote", 1);
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

void	ms_remove_quotes(t_ms *ms)
{
	t_list	*aux;
	char	*tmp;
	int		count;

	aux = ms->tokens;
	tmp = ft_strdup((char *)aux->content);
	gc_add(tmp, &ms->gc);
	while (aux)
	{
		if (ft_strchr(tmp, S_QUOTE) || ft_strchr(tmp, D_QUOTE))
		{
			count = ms_count_quotes(tmp);
			aux->content = ms_trim_quotes(ms, tmp, count);
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
	ms_print_list(ms->tokens);
	ft_lstclear(&ms->tokens, free);
	return (TRUE);
}
