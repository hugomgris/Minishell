/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 19:31:20 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_handle_system_cmd(t_ms *ms, char **env)
{
	if (ms->input[0] != '$' && (ms->cmd_args[0][0] == '/'
		|| ms->cmd_args[0][0] == '.'))
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
		free(ms->cmd_args);
		return (ms_error_handler(ms, "Error: Command not found", 0), 1);
	}
	ms_error_handler(ms, "Error: execve failed", 0);
	free(ms->filt_args);
	free(ms->cmd_args);
	return (-1);
}

void	ms_handle_builtin(t_ms *ms, char **env, int saved_fds[3])
{
	ms_save_std_fds(saved_fds);
	if (ms_has_redirection(ms))
		ms_redirection(ms);
	ms_exec_command(ms, env);
	ms_restore_std_fds(saved_fds);
	ms_cleanup_heredoc(ms);
}

void	ms_process_command(t_ms *ms, char **env, int i)
{
	pid_t	pid;
	int		saved_fds[3];

	if (ms_has_heredoc(ms) && ms_handle_heredoc_setup(ms) == -1)
	{
		if (ms_get_set(0, 0) == 2)
			ms_error_handler(ms, "Error: Failed to redir input", 0);
		return ;
	}
	if (ms->filt_args[0] && ms_is_builtin(ms->filt_args[0]) && !ms->pipe_count)
		return (ms_handle_builtin(ms, env, saved_fds));
	if (!ft_array_count(ms->filt_args))
		return ;
	pid = fork();
	if (pid == 0)
		ms_handle_child_process(ms, env, i);
	else if (pid > 0)
		ms_handle_parent_process(ms);
}

void	ms_execute_chunk(t_ms *ms, char **env, int i)
{
	int	arg_count;

	ms->cmd_args = ms_parse_args(ms->exec_chunks[i], &arg_count);
	ms_filter_args(ms);
	ms_process_command(ms, env, i);
	ms_cleanup_args(ms);
}

int	ms_executor(t_ms *ms)
{
	char	**env;
	int		i;

	if (!ft_lstsize(ms->tokens))
		return (1);
	ms_initialize_execution(ms, &env);
	i = -1;
	while (++i < ft_array_count(ms->exec_chunks))
	{
		ms_execute_chunk(ms, env, i);
		if (i < ft_array_count(ms->exec_chunks) - 1)
			ms_close_used_pipes(ms->pipe_fds, i);
	}
	ms_close_parent_pipes(ms->pipe_fds, ms->pipe_count);
	ms_wait_children(ft_array_count(ms->exec_chunks));
	ms_executor_cleanup(ms, env);
	ms_free_pipes(ms->pipe_fds, ms->pipe_count);
	return (0);
}
