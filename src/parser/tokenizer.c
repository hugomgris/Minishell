/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/02 18:32:39 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_operator(char *c)
{
	if (*c == '<' || *c == '>' || *c == '|' || *c == '&'
		|| *c == '(' || *c == ')' || *c == '\t')
		return (TRUE);
	return (FALSE);
}

void	ms_skip_quotes(char	*str, int *i)
{
	char	quote;

	quote = str[*i];
	(*i)++;
	while (str[*i] != quote)
	{
		(*i)++;
	}
	(*i)++;
}

void	ms_extract_atom(char **str)
{
	int		i;
	char	*token;
	char	*tmp;

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
	(*str) += i;
	printf("%s\n", token);
}

void	ms_extract_operator(t_token_type type, char **str)
{
	char	*token;

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
	printf("%s\n", token);
}

void	ms_skip_space(char **str)
{
	while (**str && ft_isspace(**str))
		(*str)++;
}

void	ms_handle_operator(char **str)
{
	if (!ft_strncmp(*str, "<<", 2))
		ms_extract_operator(T_DBLESS, str);
	if (!ft_strncmp(*str, ">>", 2))
		ms_extract_operator(T_DBGREATER, str);
	if (!ft_strncmp(*str, "||", 2))
		ms_extract_operator(T_OR, str);
	if (!ft_strncmp(*str, "&&", 2))
		ms_extract_operator(T_AND, str);
	if (!ft_strncmp(*str, "<", 1))
		ms_extract_operator(T_LESS, str);
	if (!ft_strncmp(*str, ">", 1))
		ms_extract_operator(T_GREATER, str);
	if (!ft_strncmp(*str, "|", 1))
		ms_extract_operator(T_PIPE, str);
	if (!ft_strncmp(*str, "(", 1))
		ms_extract_operator(T_LPARENTH, str);
	if (!ft_strncmp(*str, ")", 1))
		ms_extract_operator(T_RPARENTH, str);
}

void	ms_tokenizer(t_ms *ms, char *str)
{
	while (*str)
	{
		if (ft_isspace(*str))
			ms_skip_space(&str);
		if (is_operator(str))
			ms_handle_operator(&str);
		else 
			ms_extract_atom(&str);
	}
	if (!ms_syntax_checker(ms, str))
		return ;
	str = ms_expand_variable(ms, str);
}
