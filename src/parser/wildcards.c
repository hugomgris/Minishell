/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 11:40:33 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/19 15:46:49 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	//printf("%s\n", fname->d_name);
	closedir(dir);
}
