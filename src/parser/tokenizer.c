/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/04 17:51:07 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	TODO
	Protect code in case of wrong operator syntax:
	bash: syntax error near unexpected token `<<<'
	bash: syntax error near unexpected token `>>'
	etc.
*/

void	ms_extract_atom(t_ms *ms, char **str)
{
	int		i;
	char	*token;
	char	*tmp;
	t_list	*node;

	i = 0;
	tmp = *str;
	while (tmp[i] && !is_operator(tmp + i) && !ft_isspace(tmp[i]))
	{
		if (!ft_strncmp(tmp, "\"", 1) || !ft_strncmp(tmp, "'", 1))
			ms_skip_quotes(tmp, &i);
		else
			i++;
	}
	token = ft_substr(tmp, 0, i);
	node = ft_lstnew(token);
	ft_lstadd_back(&ms->tokens, node);
	(*str) += i;
}

void	ms_extract_operator(t_ms *ms, t_token_type type, char **str)
{
	char	*token;
	t_list	*node;

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
	ft_lstadd_back(&ms->tokens, node);
}

void	ms_handle_operator(t_ms *ms, char **str)
{
	if (!ft_strncmp(*str, "<<", 2))
		ms_extract_operator(ms, T_DBLESS, str);
	if (!ft_strncmp(*str, ">>", 2))
		ms_extract_operator(ms, T_DBGREATER, str);
	if (!ft_strncmp(*str, "||", 2))
		ms_extract_operator(ms, T_OR, str);
	if (!ft_strncmp(*str, "&&", 2))
		ms_extract_operator(ms, T_AND, str);
	if (!ft_strncmp(*str, "<", 1))
		ms_extract_operator(ms, T_LESS, str);
	if (!ft_strncmp(*str, ">", 1))
		ms_extract_operator(ms, T_GREATER, str);
	if (!ft_strncmp(*str, "|", 1))
		ms_extract_operator(ms, T_PIPE, str);
	if (!ft_strncmp(*str, "(", 1))
		ms_extract_operator(ms, T_LPARENTH, str);
	if (!ft_strncmp(*str, ")", 1))
		ms_extract_operator(ms, T_RPARENTH, str);
}

void	ms_tokenizer(t_ms *ms, char *str)
{
	ms->tokens = NULL;
	while (*str)
	{
		if (ft_isspace(*str))
		{
			while (*str && ft_isspace(*str))
				str++;
		}
		if (is_operator(str))
			ms_handle_operator(ms, &str);
		else
			ms_extract_atom(ms, &str);
	}
	if (!ms_syntax_checker(ms, str))
		return ;
	str = ms_expand_variable(ms, str);
}
