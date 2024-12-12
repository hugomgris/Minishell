/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/12 22:52:00 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_extract_atom(t_ms *ms, char **str)
{
	int		i;
	char	*token;
	char	*tmp;
	t_list	*node;

	i = 0;
	tmp = *str;
	while (tmp[i] && !is_operator(tmp + i) && !ft_isspace(tmp[i]))
	{
		if (tmp[i] == 34 || tmp[i] == 39)
		{
			if (!ms_skip_quotes(ms, tmp, &i))
				return (TRUE);
		}
		else
			i++;
	}
	token = ft_substr(tmp, 0, i);
	node = ft_lstnew(token);
	if (!token || !node)
		ms_error_handler(ms, "Error: Malloc failed allocating a token", 1);
	ft_lstadd_back(&ms->tokens, node);
	(*str) += i;
	return (FALSE);
}

int	ms_extract_operator(t_ms *ms, t_token_type type, char **str)
{
	char	*token;
	t_list	*node;

	if (ms_check_operator(ms, str))
		return (TRUE);
	if (type == T_DBGREATER || type == T_DBLESS
		|| type == T_AND || type == T_OR)
	{
		token = ft_substr(*str, 0, 2);
		*str += 2;
	}
	else
	{
		token = ft_substr(*str, 0, 1);
		(*str)++;
	}
	node = ft_lstnew(token);
	if (!token || !node)
		ms_error_handler(ms, "Error: Malloc failed allocating a token", 1);
	ft_lstadd_back(&ms->tokens, node);
	return (FALSE);
}

/*
	TODO
	Add rules to match only if token is ">>" and not ">>dsa" for example
*/

int	ms_handle_operator(t_ms *ms, char **str)
{
	if (!ft_strncmp(*str, "<<", 2))
		return (ms_extract_operator(ms, T_DBLESS, str));
	if (!ft_strncmp(*str, ">>", 2))
		return (ms_extract_operator(ms, T_DBGREATER, str));
	if (!ft_strncmp(*str, "||", 2))
		return (ms_extract_operator(ms, T_OR, str));
	if (!ft_strncmp(*str, "&&", 2))
		return (ms_extract_operator(ms, T_AND, str));
	if (**str == '&')
		return (ms_extract_operator(ms, T_AMPERSAND, str));
	if (**str == '<')
		return (ms_extract_operator(ms, T_LESS, str));
	if (**str == '>')
		return (ms_extract_operator(ms, T_GREATER, str));
	if (**str == '|')
		return (ms_extract_operator(ms, T_PIPE, str));
	if (**str == '(')
		return (ms_extract_operator(ms, T_LPARENTH, str));
	if (**str == ')')
		return (ms_extract_operator(ms, T_RPARENTH, str));
	return (FALSE);
}

int	ms_tokenizer(t_ms *ms, char *str)
{
	int	error;

	error = 0;
	while (*str)
	{
		if (error)
		{
			ft_lstclear(&ms->tokens, free);
			return (FALSE);
		}
		while (*str && ft_isspace(*str))
			str++;
		if (is_operator(str))
			error = ms_handle_operator(ms, &str);
		else
			error = ms_extract_atom(ms, &str);
	}
	return (TRUE);
}
