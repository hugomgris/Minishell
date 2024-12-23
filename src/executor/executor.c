/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/20 13:11:31 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_handle_system_cmd(t_ms *ms, char **env)
{
	if (ms->cmd_args[0][0] == '/' || ms->cmd_args[0][0] == '.')
	{
		if (ms_exec_direct_path(ms, ms->filt_args, env))
		{
			ft_free(ms->filt_args);
			ft_free(ms->cmd_args);
			return (1);
		}
	}
	else if (ms_search_in_path(ms, ms->filt_args, env))
	{
		free(ms->filt_args);
		ft_free(ms->cmd_args);
		return (ms_error_handler(ms, "Error: Command not found", 0), 1);
	}
	ms_error_handler(ms, "Error: execve failed", 0);
	ft_free(ms->filt_args);
	ft_free(ms->cmd_args);
	return (-1);
}

// Main execution function
int	ms_exec_command(t_ms *ms, char **env)
{
	if (!ms->cmd_args)
	{
		ms_error_handler(ms, "Error: Mem alloc failed", 1);
		return (1);
	}
	if (ms_has_redirection(ms))
	{
		if (ms_redirection(ms) == -1)
		{
			free(ms->cmd_args);
			return (1);
		}
	}
	if (ms_is_builtin(ms->cmd_args[0]))
	{
		if (ms_reroute_builtins(ms, env))
		{
			free(ms->cmd_args);
			free(ms->filt_args);
			return (1);
		}
	}
	else if (ms_handle_system_cmd(ms, env) == -1)
	{
		ft_free(ms->cmd_args);
		ft_free(ms->filt_args);
		return (1);
	}
	return (1);
}

/*
Executor hub.
TODO: handle builtins and system correctly and separatedly
*/
void	ms_executor(t_ms *ms)
{
	char	**env;
	pid_t	pid;
	int		i;
	int		arg_count;

	ms->exec_chunks = ms_extract_chunks(ms, &ms->tokens);
	env = ms_rebuild_env(ms);
	ms->pipe_count = ft_array_count(ms->exec_chunks) - 1;
	ms_create_pipes(ms, &ms->pipe_fds, ms->pipe_count);
	i = -1;
	while (++i < ft_array_count(ms->exec_chunks))
	{
		ms->cmd_args = ms_parse_args(ms->exec_chunks[i], &arg_count);
		ms_filter_args(ms);
		if (ms_is_builtin(ms->filt_args[0]))
			ms_exec_command(ms, env);
		else
		{
			pid = fork();
			if (pid == 0)
			{
				ms_setup_child_pipes(ms->pipe_fds, i, ms->pipe_count);
				ms_close_child_pipes(ms->pipe_fds, ms->pipe_count);
				ms_exec_command(ms, env);
				exit(EXIT_FAILURE);
			}
		}
		free(ms->filt_args);
		free(ms->cmd_args);
	}
	ms_close_parent_pipes(ms->pipe_fds, ms->pipe_count);
	ms_wait_children(ft_array_count(ms->exec_chunks));
	ms_executor_cleanup(ms, env);
	ms_free_pipes(ms->pipe_fds, ms->pipe_count);
}
