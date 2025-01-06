/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/03 09:57:06 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Parses the execution chunk into an array of arguments.
Steps:
  1. Allocates memory for the array of strings to store arguments.
  2. Splits the exec_chunk into tokens using whitespace as a delimiter.
  3. Counts the number of arguments and stores them in arg_count.
  4. Null-terminates the array.
*/
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

/*
Processes directory tokens for command path search.
Steps:
  1. Checks if the directory pointer is NULL, indicating end of processing.
  2. Frees the path_copy and returns NULL in case of no valid directories.
  3. Proceeds to tokenize the next directory path for subsequent searches.
  4. Returns pointer to the next directory token on success.
*/
char	*ms_process_directory(char **path_copy, char **dir)
{
	if (!*dir)
	{
		free(*path_copy);
		return (NULL);
	}
	return (ft_strtok(NULL, ":"));
}

/*
Attempts to execute a command at a given path.
Steps:
  1. Tries to execute the command located at cmd_path.
  2. If execution succeeds, frees allocated memory and returns 0.
  3. On failure, frees the command path and returns 1.
*/
int	ms_try_and_execute(char *cmd_path, char **cmd_args, char **env, char *path)
{
	(void)path;
	return (ms_try_path_execution(cmd_path, cmd_args, env));
}

/*
Searches for a cmd in the directories listed in the PATH environment variable.
Steps:
  1. Duplicates the PATH variable for safe tokenization.
  2. Iterates through directories in the PATH, building potential command paths.
  3. Attempts to execute the command in each directory.
  4. Frees resources and returns based on success (0) or failure(1).
*/
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
		cmd_path = ms_build_cmd_path(ms, dir, cmd_args[0]);
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
