/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:25:37 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/15 11:47:30 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_process_quotes(t_ms *ms, char **tmp, t_token **subtok, char quote)
{
	t_token	*new;
	char	*str;
	int		i;

	i = 1;
	while ((*tmp)[i] && (*tmp)[i] != quote)
		i++;
	if ((*tmp)[i] == quote)
		i++;
	str = ft_substr(*tmp, 0, i);
	if (!str)
		ms_error_handler(ms, "Malloc failed expanding a variable", 1);
	new = ms_new_token(str, T_ATOM);
	if (!new)
		ms_error_handler(ms, "Malloc failed expanding a variable", 1);
	ms_tokadd_back(subtok, new);
	*tmp += i;
}

void	ms_process_unquoted(t_ms *ms, char **tmp, t_token **subtok)
{
	t_token	*new;
	char	*str;
	int		i;

	i = 0;
	while ((*tmp)[i] && !ms_is_quote((*tmp)[i]))
	{
		i++;
		if ((*tmp)[i] == '$')
			break ;
	}
	str = ft_substr(*tmp, 0, i);
	if (!str)
		ms_error_handler(ms, "Malloc failed expanding a variable", 1);
	new = ms_new_token(str, T_ATOM);
	if (!new)
		ms_error_handler(ms, "Malloc failed expanding a variable", 1);
	ms_tokadd_back(subtok, new);
	*tmp += i;
}

void	ms_process_escape(t_ms *ms, char **tmp, t_token **subtok, char quote)
{
	t_token	*new;
	char	*str;
	int		i;

	i = 2;
	while ((*tmp)[i] && (*tmp)[i] != quote)
		i++;
	if ((*tmp)[i] == quote)
		i++;
	str = ft_substr(*tmp, 0, i);
	if (!str)
		ms_error_handler(ms, "Malloc failed expanding a variable", 1);
	new = ms_new_token(str, T_ATOM);
	if (!new)
		ms_error_handler(ms, "Malloc failed expanding a variable", 1);
	ms_tokadd_back(subtok, new);
	*tmp += i;
}

/*
	Processes the content of a token in order to expand its $ variable,
	depending if it is quoted or not.
	Also excludes the unsupported '$"VAR"' syntax.
*/
void	ms_process_token_content(t_ms *ms, char *tmp, t_token **subtok)
{
	while (*tmp)
	{
		if (*tmp == D_QUOTE || *tmp == S_QUOTE)
			ms_process_quotes(ms, &tmp, subtok, *tmp);
		else if (*tmp == '$' && (*(tmp + 1) == S_QUOTE || \
			*(tmp + 1) == D_QUOTE))
			ms_process_escape(ms, &tmp, subtok, *tmp);
		else
			ms_process_unquoted(ms, &tmp, subtok);
	}
}
