/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:06:53 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/06 13:08:35 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Returns the number of filenames in current dir matching a pattern.
*/
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
