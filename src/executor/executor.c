/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/16 09:21:43 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Handles the execution of system commands by determining
	the type of command path provided.
- If the command is an absolute path (starts with '/'),
	it delegates to ms_handle_absolute_path.
- If the command is a relative path (starts with '.' or contains '/'),
	it delegates to ms_handle_relative_path.
- For commands without a path, it searches for the command in the
	system's PATH environment variable:
  - If found, it delegates to ms_handle_relative_path.
  - If not found, it cleans up resources and returns an error
  	indicating the command was not found.
Returns the exit status of the command execution or 1 on error.
*/
int	ms_handle_system_cmd(t_ms *ms, char **env)
{
	if (ms->filt_args[0][0] == '/')
		return (ms_handle_absolute_path(ms, env));
	else if (ms->filt_args[0][0] == '.' || ft_strchr(ms->filt_args[0], '/'))
		return (ms_handle_relative_path(ms, env));
	else
	{
		if (ms_ex_check_file_in_dir(ms->filt_args[0]))
			return (ms_handle_relative_path(ms, env));
		else if (ms_search_in_path(ms, ms->filt_args, env))
			return (ms_handle_relative_path(ms, env));
		else
		{
			ft_free(ms->filt_args);
			ft_free(ms->cmd_args);
			return (ms_error_handler(ms, "Error: Command not found", 0), 1);
		}
	}
}

/*
Handles the execution of built-in commands.
Saves the current state of standard FDs before applying redirections.
Executes the built-in command with its arguments.
Restores standard file descriptors after execution.
Performs cleanup for any heredoc state set during execution.
*/
int	ms_handle_builtin(t_ms *ms, char **env, int saved_fds[3])
{
	int	code;

	ms_save_std_fds(saved_fds);
	code = ms_exec_command(ms, env);
	ms_restore_std_fds(saved_fds);
	ms_cleanup_heredoc(ms);
	return (code);
}

/*
Processes individual commands within a pipeline or as standalone.
Handles heredoc input setup if required.
Checks for built-in cmds (executed directly) or forks to execute other cmds.
Manages child and parent process behavior during forking:
  - Child: Executes the command.
  - Parent: Waits or manages pipes.
*/
int	ms_process_command(t_ms *ms, char **env, int i)
{
	pid_t	pid;
	int		saved_fds[3];

	if (ms_has_heredoc(ms) && ms_handle_heredoc_setup(ms) == -1)
	{
		if (ms_get_set(GET, 0) == 2)
			ms_error_handler(ms, "Error: Failed to redir input", 0);
		return (1);
	}
	if (ms_get_set(GET, 0) == SHELL_HEREDOC_INTERRUPTED)
		return (130);
	if (ms->filt_args[0] && ms_is_builtin(ms->filt_args[0]) && !ms->pipe_count)
		return (ms_handle_builtin(ms, env, saved_fds));
	if (!ft_array_count(ms->filt_args))
		return (0);
	pid = fork();
	if (pid == 0)
	{
		ms_handle_child_process(ms, env, i);
		exit(0);
	}
	else if (pid > 0)
		return (ms_wait_children(ms, 1));
	else
		return (ms_error_handler(ms, "minishell: fork failed", 0), 1);
}

/*
Executes a single chunk of commands parsed from input.
Parses arguments and applies filtering to handle special tokens.
Delegates execution to ms_process_command.
Cleans up parsed arguments and filtered states after execution.
*/
int	ms_execute_chunk(t_ms *ms, char **env, int i)
{
	int		arg_count;
	int		code;
	char	*warning;
	char	*sep;

	warning = "\033[1;91mRedirection: Warning: Chained heredoc redirections "
		"detected, will only consider last with separator:\033[1;38;5;214m ";
	sep = ms_get_separator(ms);
	warning = ft_strjoin3(warning, sep, "\033[1;91m.\033[0m");
	gc_add(warning, &ms->gc);
	ms->cmd_args = ms_parse_args(ms, ms->exec_chunks[i], &arg_count);
	ms_filter_args(ms);
	if (ft_array_count(ms->filt_args) == 0)
	{
		if (ms_detect_redirector(ms->cmd_args[0]))
			ms_error_handler(ms, "redirection: command needed", 0);
		ft_free(ms->filt_args);
		free(ms->cmd_args);
		return (0);
	}
	if (ms_has_heredoc(ms) > 1)
		printf("%s\n", warning);
	code = ms_process_command(ms, env, i);
	ms_cleanup_args(ms);
	return (code);
}

/*
Manages the overall execution flow of the Minishell.
Initializes the environment and prepares execution chunks.
Iterates through each execution chunk, managing pipes and redirections.
Waits for all child processes to finish execution.
Performs cleanup for pipes, environment, and other resources after execution.
Returns 0 on successful execution.
*/
int	ms_executor(t_ms *ms)
{
	char	**env;
	int		i;
	int		count;

	if (!ms_toksize(ms->chain_tokens))
		return (1);
	ms_initialize_execution(ms, &env);
	i = -1;
	count = ms_count_chunks(ms, ms->chain_tokens);
	while (++i < count)
	{
		ms->exit_status = ms_execute_chunk(ms, env, i);
		if (i < count - 1)
			ms_close_used_pipes(ms->pipe_fds, i);
	}
	ms_close_parent_pipes(ms->pipe_fds, ms->pipe_count);
	if (ms_get_set(GET, 0) == 3)
		return (ms_heredoc_interruption(ms, env));
	if (count > 1)
		ms_wait_children(ms, count);
	ms_executor_cleanup(ms, env);
	ms_free_pipes(ms->pipe_fds, ms->pipe_count);
	ms_cleanup_heredoc(ms);
	return (ms->exit_status);
}
