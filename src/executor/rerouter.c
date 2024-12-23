/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rerouter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/23 16:10:50 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Check if the command is a builtin
int	ms_is_builtin(const char *cmd)
{
	return (ft_strncmp(cmd, "cd", 2) == 0
		|| ft_strncmp(cmd, "echo", 4) == 0
		|| ft_strncmp(cmd, "env", 3) == 0
		|| ft_strncmp(cmd, "pwd", 3) == 0
		|| ft_strncmp(cmd, "unset", 5) == 0
		|| ft_strncmp(cmd, "exit", 4) == 0
		|| ft_strncmp(cmd, "export", 6) == 0);
}

// Reroute to the correct builtin function
int	ms_reroute_builtins(t_ms *ms, char **env)
{
	if (ft_strncmp(ms->filt_args[0], "cd", 2) == 0)
		return (ms_cd(ms));
	if (ft_strncmp(ms->filt_args[0], "echo", 4) == 0)
		return (ms_echo(ms->filt_args));
	else if (ft_strncmp(ms->filt_args[0], "env", 3) == 0)
		return (ms_env(ms));
	if (ft_strncmp(ms->filt_args[0], "pwd", 3) == 0)
		return (ms_pwd(ms));
	else if (ft_strncmp(ms->filt_args[0], "unset", 5) == 0)
		return (ms_unset(ms));
	else if (ft_strncmp(ms->filt_args[0], "exit", 4) == 0)
		return (ms_exit(ms));
	else if (ft_strncmp(ms->filt_args[0], "export", 6) == 0)
		return (ms_export(ms, ms->filt_args, env));
	return (0);
}
