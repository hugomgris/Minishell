/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 11:40:33 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/06 12:52:32 by nponchon         ###   ########.fr       */
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

int	ms_process_dir_entry(t_ms *ms, char *pat, t_token *sub, struct dirent *ent)
{
	int		flag;
	char	*tmp;
	t_token	*new;

	flag = 0;
	if (ms_match_pattern(pat, ent->d_name))
	{
		flag = 1;
		tmp = ft_strdup(ent->d_name);
		new = ms_new_token(tmp, T_ATOM);
		if (!new || !tmp)
			ms_exit_handler(ms, "Malloc failed creating a wildcard", 1);
		ms_tokinsert(&ms->tok, sub, new);
	}
	return (flag);
}

void	ms_wildcard_unmatched(t_ms *ms, char *pattern)
{
	char	*tmp;

	tmp = ft_strjoin("no matches found: ", pattern);
	gc_add(tmp, &ms->gc);
	ms_error_handler(ms, tmp, 0);
}

void	ms_get_wildcards(t_ms *ms, char *pattern, t_token *subtoken)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(".");
	if (!dir)
		ms_exit_handler(ms, "Error opening a DIR", 1);
	entry = readdir(dir);
	while (entry)
	{
		ms_process_dir_entry(ms, pattern, subtoken, entry);
		entry = readdir(dir);
	}
	if (!ms_toksize(subtoken))
		ms_wildcard_unmatched(ms, pattern);
	else
	{
		free(subtoken->content);
		subtoken->content = ft_strdup("");
	}
	if (!subtoken->content)
		ms_exit_handler(ms, "Malloc failed creating a wildcard", 1);
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
			free(tmp);
		}
		aux = aux->next;
	}
}
