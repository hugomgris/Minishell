/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/16 09:23:45 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Creates an error message for file-related errors
	(e.g., permission denied or file not found).
- Trims leading './' from the command name for cleaner error messages.
- Constructs an appropriate error message based on the value of `errno`:
  - If `errno` is `EACCES`, the error message indicates "Permission denied".
  - Otherwise, the error message indicates "No such file or directory".
- Adds the error message to the garbage collector for automatic cleanup.
Returns the constructed error message.
*/
char	*ms_create_error_message(t_ms *ms, char *cmd)
{
	char	*file;
	char	*error_msg;

	file = ft_strdup(cmd);
	gc_add(file, &ms->gc);
	file = ft_strtrim(file, "./");
	gc_add(file, &ms->gc);
	if (errno == EACCES)
		error_msg = ft_strjoin(file, ": Permission denied");
	else
		error_msg = ft_strjoin(file, ": No such file or directory");
	gc_add(error_msg, &ms->gc);
	return (error_msg);
}

/*
Executes a command specified by a relative path.
- Normalizes the path to resolve any relative components (e.g., './' or '../').
- Attempts to execute the command using `execve`.
- If execution fails, creates an error message using `ms_create_error_message`
	and handles the error.
Returns 1 on failure, or does not return on success (replaced by `execve`).
*/
int	ms_exec_relative_path(t_ms *ms, char **cmd_args, char **env)
{
	struct stat	stat_buf;
	char		*file;
	char		*path;

	path = ms_normalize_path(ms, cmd_args[0]);
	if (stat(path, &stat_buf) == 0)
		execve(path, cmd_args, env);
	file = ms_create_error_message(ms, cmd_args[0]);
	ms_error_handler(ms, file, 0);
	return (1);
}

/*
Handles the search for a command in the system's PATH.
- Attempts to locate the command using `ms_search_in_path`.
- If the command is not found, frees resources and returns an error.
- If `execve` fails, frees resources and returns an error.
Returns 1 if the command is not found, or -1 if `execve` fails.
*/
int	ms_handle_path_search(t_ms *ms, char **env)
{
	if (ms_search_in_path(ms, ms->filt_args, env))
	{
		ft_free(ms->filt_args);
		ft_free(ms->cmd_args);
		return (ms_error_handler(ms, "Error: Command not found", 0), 1);
	}
	ms_error_handler(ms, "Error: execve failed", 0);
	ft_free(ms->filt_args);
	ft_free(ms->cmd_args);
	return (-1);
}

/*
Handles the execution of a command specified by a relative path.
- Prepends './' to the command name if it is not already
	an absolute or relative path.
- Attempts to execute the command using `ms_exec_relative_path`.
- If execution fails, frees resources and returns an error.
Returns 1 on failure, or -1 if `execve` fails.
*/
int	ms_handle_relative_path(t_ms *ms, char **env)
{
	char	*path;

	if (ms->filt_args[0][0] != '/' && ms->filt_args[0][0] != '.')
		path = ft_strjoin("./", ms->filt_args[0]);
	else
		path = ft_strdup(ms->filt_args[0]);
	if (ms_exec_relative_path(ms, ms->filt_args, env))
	{
		free(path);
		ft_free(ms->filt_args);
		return (1);
	}
	free(path);
	ms_error_handler(ms, "Error: execve failed", 0);
	ft_free(ms->filt_args);
	return (-1);
}

/*
Handles the execution of a command specified by an absolute path.
- Attempts to execute the command using `ms_exec_direct_path`.
- If execution fails, frees resources and returns an error.
Returns 1 on failure, or -1 if `execve` fails.
*/
int	ms_handle_absolute_path(t_ms *ms, char **env)
{
	if (ms_exec_direct_path(ms, ms->filt_args, env))
	{
		ft_free(ms->filt_args);
		return (1);
	}
	ms_error_handler(ms, "Error: execve failed", 0);
	ft_free(ms->filt_args);
	return (-1);
}
