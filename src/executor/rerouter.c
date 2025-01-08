/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rerouter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/08 11:47:47 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Determines if the given command is a built-in.
If the command matches any of the built-in names, the function returns 1. 
Otherwise, it returns 0.
*/
int	ms_is_builtin(const char *cmd)
{
	return ((!ft_strncmp(cmd, "cd", 2) && ft_strlen(cmd) == 2)
		|| (!ft_strncmp(cmd, "echo", 4) && ft_strlen(cmd) == 4)
		|| (!ft_strncmp(cmd, "env", 3) && ft_strlen(cmd) == 3)
		|| (!ft_strncmp(cmd, "pwd", 3) && ft_strlen(cmd) == 3)
		|| (!ft_strncmp(cmd, "unset", 5) && ft_strlen(cmd) == 5)
		|| (!ft_strncmp(cmd, "exit", 4) && ft_strlen(cmd) == 4)
		|| (!ft_strncmp(cmd, "export", 6) && ft_strlen(cmd) == 6));
}

/*
Routes the execution flow to the appropriate built-in command handler.
The corresponding handler function is called and its return value is returned. 
If the command is not a built-in, the function returns 0, indicating
	the command should be handled elsewhere.
*/
int	ms_reroute_builtins(t_ms *ms, char **env)
{
	char	*arg;

	arg = ms->filt_args[0];
	if (!ft_strncmp(arg, "cd", 2) && ft_strlen(arg) == 2)
		return (ms_cd(ms));
	else if (!ft_strncmp(arg, "echo", 4) && ft_strlen(arg) == 4)
		return (ms_echo(ms->filt_args));
	else if (!ft_strncmp(arg, "env", 3) && ft_strlen(arg) == 3)
		return (ms_env(ms));
	else if (!ft_strncmp(arg, "pwd", 3) && ft_strlen(arg) == 3)
		return (ms_pwd(ms));
	else if (!ft_strncmp(arg, "unset", 5) && ft_strlen(arg) == 5)
		return (ms_unset(ms));
	else if (!ft_strncmp(arg, "exit", 4) && ft_strlen(arg) == 4)
		return (ms_exit(ms));
	else if (!ft_strncmp(arg, "export", 6) && ft_strlen(arg) == 6)
		return (ms_export(ms, ms->filt_args, env));
	return (0);
}
