/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 21:07:08 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Executes a command at a direct path.
Steps:
  1. Checks if the file at cmd_args[0] exists and is a valid file using stat().
  2. If the file exists, attempts to execute it using execve.
  3. If execution fails, outputs an error using ms_error_handler.
  4. Returns 0 on success, 1 on failure (invalid command or exec fail).
*/
int	ms_exec_direct_path(t_ms *ms, char **cmd_args, char **env)
{
	struct stat	stat_buf;
	char		*file;

	if (stat(cmd_args[0], &stat_buf) == 0)
		execve(cmd_args[0], cmd_args, env);
	file = ft_strdup(cmd_args[0]);
	gc_add(file, &ms->gc);
	file = ft_strtrim(file, "./");
	gc_add(file, &ms->gc);
	if (errno == EACCES)
	{
		file = ft_strjoin(file, ": Permission denied");
		gc_add(file, &ms->gc);
	}
	else
	{
		file = ft_strjoin(file, ": No such file or directory");
		gc_add(file, &ms->gc);
	}
	ms_error_handler(ms, file, 0);
	return (1);
}

/*
Attempts to execute a command at a specified path.
Steps:
  1. Checks if the cmd_path exists and is executable using stat().
  2. If the file exists and is executable, tries to execute it using execve.
  3. Returns 0 on exec failure or non-executable file.
  4. Doesn't return on success, as execve replaces current process.
*/
int	ms_try_path_execution(char *cmd_path, char **cmd_args, char **env)
{
	struct stat	stat_buf;

	if (stat(cmd_path, &stat_buf) == 0 && (stat_buf.st_mode & S_IXUSR))
	{
		execve(cmd_path, cmd_args, env);
	}
	return (1);
}

/*
Builds the full command path by combining the directory and the command name.
Steps:
  1. Allocates memory for the full command path.
  2. Concatenates the directory and command name with a "/" separator.
  3. Returns a pointer to the full command path on success.
*/
char	*ms_build_cmd_path(t_ms *ms, char *dir, char *cmd)
{
	char	*cmd_path;

	cmd_path = malloc(strlen(dir) + strlen(cmd) + 2);
	if (!cmd_path)
		return (NULL);
	gc_add(cmd_path, &ms->gc);
	ft_strlcpy(cmd_path, dir, PATH_MAX);
	gc_add(cmd_path, &ms->gc);
	ft_strlcat(cmd_path, "/", PATH_MAX);
	gc_add(cmd_path, &ms->gc);
	ft_strlcat(cmd_path, cmd, PATH_MAX);
	gc_add(cmd_path, &ms->gc);
	return (cmd_path);
}

/*
Duplicates the PATH environment variable for use in command search.
Steps:
  1. Retrieves the PATH variable using ms_get_env_variable().
  2. If PATH is not set, defines a default value.
  3. Returns the PATH variable (default or retrieved).
*/
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
