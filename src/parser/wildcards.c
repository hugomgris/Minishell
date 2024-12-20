/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 11:40:33 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/20 17:22:28 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_match_pattern(char *str, char *entry)
{
	(void)str;
	(void)entry;
	return (0);
}

void	ms_get_wildcards(t_ms *ms)
{
	DIR				*dir;
	struct dirent	*fname;
	char			*filename;

	filename = ms_get_env_variable(ms, "PWD");
	dir = opendir(filename);
	if (!dir)
		ms_error_handler(ms, "Error opening current directory", 1);
	fname = readdir(dir);
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

void	ms_expand_wildcards(t_ms *ms, t_token *subtok)
{
	int	count;

	(void)ms;
	cucufu(42);
	count = ms_match_count((char *)subtok->content);
	printf("count=%i\n", count);
}
