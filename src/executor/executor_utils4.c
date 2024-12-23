/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/23 12:05:44 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains 
*/

#include "../../includes/minishell.h"

// Helper function to execute direct path commands
int	ms_exec_direct_path(t_ms *ms, char **cmd_args, char **env)
{
	struct stat	stat_buf;

	if (stat(cmd_args[0], &stat_buf) == 0)
		execve(cmd_args[0], cmd_args, env);
	ms_error_handler(ms, "Error: execve: invalid file", 0);
	return (1);
}

// Helper function to try executing command with given path
int	ms_try_path_execution(char *cmd_path, char **cmd_args, char **env)
{
	struct stat	stat_buf;

	if (stat(cmd_path, &stat_buf) == 0 && (stat_buf.st_mode & S_IXUSR))
		execve(cmd_path, cmd_args, env);
	return (0);
}

// Helper function to build full command path
char	*ms_build_cmd_path(char *dir, char *cmd)
{
	char	*cmd_path;

	cmd_path = malloc(strlen(dir) + strlen(cmd) + 2);
	if (!cmd_path)
		return (NULL);
	ft_strlcpy(cmd_path, dir, PATH_MAX);
	ft_strlcat(cmd_path, "/", PATH_MAX);
	ft_strlcat(cmd_path, cmd, PATH_MAX);
	return (cmd_path);
}

char	*ms_duplicate_path(t_ms *ms)
{
	char	*path;

	path = ms_get_env_variable(ms, "PATH");
	if (!path)
	{
		path = "/bin:/usr/local/sbin:/usr/local";
		path = ft_strjoin(path, "/bin:/usr/sbin:/usr/bin:/sbin:/bin");
		gc_add(path, &ms->gc);
	}
	return (path);
}
