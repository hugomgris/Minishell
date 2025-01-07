/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 11:40:33 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/07 14:36:34 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_match_pattern(char *pattern, char *entry)
{
	int		len;

	if (*entry == '.')
		return (FALSE);
	if (*pattern == '*' && pattern[1] == '\0')
		return (TRUE);
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

	(void)sub;
	flag = 0;
	if (ms_match_pattern(pat, ent->d_name))
	{
		flag = 1;
		tmp = ft_strdup(ent->d_name);
		new = ms_new_token(tmp, T_ATOM);
		if (!new || !tmp)
			ms_exit_handler(ms, "Malloc failed creating a wildcard", 1);
		ms_tokinsert(&ms->wc, ms->wc, new);
	}
	return (flag);
}

void	ms_add_wc(t_ms *ms, t_token *sub)
{
	if (ms_toksize(ms->wc) > 1)
	{
		free(sub->content);
		sub->content = ft_strdup("");
		if (!sub->content)
			ms_exit_handler(ms, "Malloc failed creating a wildcard", 1);
		ms_tokensort(ms->wc);
		ms_tokinsert_list(&ms->tok, sub, ms->wc);
	}
	else
	{
		free(sub->content);
		sub->content = ft_strdup(ms->wc->content);
		ms_tokclear(&ms->wc, free);
		if (!sub->content)
			ms_exit_handler(ms, "Malloc failed creating a wildcard", 1);
	}
	ms->wc = NULL;
}

void	ms_get_wildcards(t_ms *ms, char *pat, t_token *sub)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(".");
	if (!dir)
		ms_exit_handler(ms, "Error opening a DIR", 1);
	entry = readdir(dir);
	while (entry)
	{
		ms_process_dir_entry(ms, pat, sub, entry);
		entry = readdir(dir);
	}
	if (ms_toksize(ms->wc))
		ms_add_wc(ms, sub);
	closedir(dir);
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
