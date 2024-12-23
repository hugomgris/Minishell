/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/21 13:05:53 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains helper/Flow-control functions for execution handling:
	-ms_parse_args
	-ms_exec_direct_path
	-ms_try_path_execution
	-ms_build_cmd_path
	-ms_search_in_path
*/

#include "../../includes/minishell.h"

char	**ms_parse_args(char *exec_chunk, int *arg_count)
{
	char	**cmd_args;
	char	*token;

	*arg_count = 0;
	cmd_args = malloc(sizeof(char *) * (strlen(exec_chunk) + 1));
	if (!cmd_args)
		return (NULL);
	token = ft_strtok(exec_chunk, " ");
	while (token)
	{
		cmd_args[(*arg_count)++] = token;
		token = ft_strtok(NULL, " ");
	}
	cmd_args[*arg_count] = NULL;
	return (cmd_args);
}

char	*ms_process_directory(char **path_copy, char **dir)
{
	if (!*dir)
	{
		free(*path_copy);
		return (NULL);
	}
	return (ft_strtok(NULL, ":"));
}

int	ms_try_and_execute(char *cmd_path, char **cmd_args, char **env, char *path)
{
	if (ms_try_path_execution(cmd_path, cmd_args, env))
	{
		free(cmd_path);
		free(path);
		return (0);
	}
	free(cmd_path);
	return (1);
}

// Helper function to search command in PATH
int	ms_search_in_path(t_ms *ms, char **cmd_args, char **env)
{
	char	*path_copy;
	char	*dir;
	char	*cmd_path;

	path_copy = ms_duplicate_path(ms);
	if (!path_copy)
		return (1);
	dir = ft_strtok(path_copy, ":");
	while (dir)
	{
		cmd_path = ms_build_cmd_path(dir, cmd_args[0]);
		if (!cmd_path)
		{
			free(path_copy);
			return (1);
		}
		if (!ms_try_and_execute(cmd_path, cmd_args, env, path_copy))
			return (0);
		dir = ms_process_directory(&path_copy, &dir);
	}
	return (1);
}
