/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/14 10:55:04 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	ms_exec_relative_path(t_ms *ms, char **cmd_args, char **env)
{
	struct stat	stat_buf;
	char		*file;
	char		*path;

	path = ms_normalize_path(ms, cmd_args[0]);
	ft_printf("path:%s\n", path);
	if (stat(path, &stat_buf) == 0)
		execve(path, cmd_args, env);
	file = ms_create_error_message(ms, cmd_args[0]);
	ms_error_handler(ms, file, 0);
	return (1);
}

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
