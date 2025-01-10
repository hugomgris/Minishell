/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils8.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/09 10:52:40 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_detect_space_arg(const char *chunk)
{
	unsigned long	i;
	int				count;

	i = 0;
	count = 0;
	while (chunk[i] && i < ft_strlen(chunk) - 2)
	{
		if (ft_isspace(chunk[i]) && ft_isspace(chunk[i + 1])
			&& ft_isspace(chunk[i + 2]))
			count++;
		i ++;
	}
	return (count);
}

/*
Helper function to handle space arguments (e.g.: tr " " "x").
It's the first of a two step process, in which spaces get 
	temporarily changed to " so that tokenization don't skip them. 
*/
char	*ms_process_space_args_in(char *chunk)
{
	unsigned long	i;

	i = 1;
	while (chunk[i] && i < ft_strlen(chunk) - 2)
	{
		if (ft_isspace(chunk[i - 1]) && ft_isspace(chunk[i])
			&& ft_isspace(chunk[i + 1]))
			chunk[i] = '"';
		i++;
	}
	return (chunk);
}

/*
Helper function to handle space arguments (e.g.: tr " " "x").
It's the second of a two step process, in which temporarily
	changed tokens (") get changed back to a space char.
*/
char	**ms_process_space_args_out(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if (args[i][0] == '"')
			args[i][0] = ' ';
	}
	return (args);
}

int	ms_heredoc_interruption(t_ms *ms, char **env)
{
	ms_get_set(SET, SHELL_NORMAL);
	ms_executor_cleanup(ms, env);
	ms_free_pipes(ms->pipe_fds, ms->pipe_count);
	ms_cleanup_heredoc(ms);
	ms->exit_status = 130;
	return (130);
}
