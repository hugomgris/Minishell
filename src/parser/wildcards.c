/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 11:40:33 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/08 11:55:16 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_match_pattern(char *pattern, char *entry, int p, int s)
{
	t_match_data	data;
	int				start;

	ms_init_match_data(&data);
	if (ms_is_hidden_entry(entry))
		return (FALSE);
	while (data.i < s)
	{
		if (data.j < p && pattern[data.j] == entry[data.i])
		{
			data.i++;
			data.j++;
		}
		else if (data.j < p && pattern[data.j] == '*')
			ms_handle_star(&data, &start);
		else if (!ms_retry_star(&data, &start))
			return (FALSE);
	}
	while (data.j < p && pattern[data.j] == '*')
		data.j++;
	return (data.j == p);
}

int	ms_process_dir_entry(t_ms *ms, char *pat, struct dirent *ent)
{
	int		flag;
	int		len_pat;
	int		len_str;
	char	*tmp;
	t_token	*new;

	flag = 0;
	len_str = ft_strlen(ent->d_name);
	len_pat = ft_strlen(pat);
	if (ms_match_pattern(pat, ent->d_name, len_pat, len_str))
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
		ms_process_dir_entry(ms, pat, entry);
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
		if (aux->type == 0 && ft_strchr(aux->content, '*'))
		{
			tmp = ft_strdup(aux->content);
			ms_get_wildcards(ms, tmp, aux);
			free(tmp);
		}
		aux = aux->next;
	}
}
