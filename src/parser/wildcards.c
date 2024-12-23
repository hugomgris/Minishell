/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 11:40:33 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/23 17:20:36 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_match_pattern(char *pattern, char *entry)
{
	int		len;

	while (*pattern == '*')
		pattern++;
	if (*pattern)
	{
		len = ft_strlen(entry);
		if (ft_strnstr(entry, pattern, len))
			return (TRUE);
	}
	return (FALSE);
}

void	ms_get_wildcards(t_ms *ms, char *pattern, t_token *subtoken)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*tmp;
	t_token			*new;
	int				flag;

	flag = 0;
	dir = opendir(".");
	if (!dir)
		ms_exit_handler(ms, "Error opening a DIR", 1);
	entry = readdir(dir);
	while (entry)
	{
		if (ms_match_pattern(pattern, entry->d_name))
		{
			flag = 1;
			tmp = ft_strdup(entry->d_name);
			new = ms_new_token(tmp, T_ATOM);
			ms_tokinsert(&ms->tok, subtoken, new);
		}
		entry = readdir(dir);
	}
	if (!flag)
		ms_error_handler(ms, \
			ft_strjoin("no matches found: ", pattern), 0);
	else
	{
		subtoken->content = ft_strdup("");
	}
	closedir(dir);
}

int	ms_match_count(char *pattern)
{
	DIR				*dir;
	int				match_count;
	struct dirent	*entry;

	match_count = 0;
	dir = opendir(".");
	entry = readdir(dir);
	while (entry)
	{
		if (ms_match_pattern(pattern, entry->d_name))
			match_count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (match_count);
}

void	ms_expand_wildcards(t_ms *ms)
{
	t_token	*aux;
	char	*tmp;

	aux = ms->tok;
	while (aux)
	{
		if (aux->type == 0 && ft_strchr((char *)aux->content, '*'))
		{
			tmp = ft_strdup((char *)aux->content);
			ms_get_wildcards(ms, tmp, aux);
			gc_add(aux->content, &ms->gc);
		}
		aux = aux->next;
	}
}
