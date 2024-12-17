/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/16 11:45:43 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Control flow function that branches builtin execution to each of the
	builtin commands' specific handler functions.
*/
void	ms_execute_builtin(t_ms *ms)
{
	size_t						i;
	char						*cmd;
	static const t_builtin_map	builtins[] = {
	{"cd", ms_cd},
	{"env", ms_env},
	{"pwd", ms_pwd},
	{"unset", ms_unset},
	{"exit", ms_exit},
	{"echo", ms_echo},
	{"export", ms_export}
	};

	cmd = ms->filtered_tokens->content;
	i = 0;
	while (i < sizeof(builtins) / sizeof(builtins[0]))
	{
		if (ft_strncmp(cmd, builtins[i].name, ft_strlen(builtins[i].name)) == 0)
		{
			builtins[i]. func(ms);
			return ;
		}
		i++;
	}
}

/*
Helper function to check if input comand is a Minishell builtin.
*/
int	ms_is_builtin(char *cmd)
{
	return ((!ft_strncmp(cmd, "cd", 2) && (!cmd[2] || cmd[2] == ' '))
		|| (!ft_strncmp(cmd, "env", 3) && (!cmd[3] || cmd[3] == ' '))
		|| (!ft_strncmp(cmd, "pwd", 3) && (!cmd[3] || cmd[3] == ' '))
		|| (!ft_strncmp(cmd, "unset", 5) && (!cmd[5] || cmd[5] == ' '))
		|| (!ft_strncmp(cmd, "exit", 4) && (!cmd[4] || cmd[4] == ' '))
		|| (!ft_strncmp(cmd, "echo", 4) && (!cmd[4] || cmd[4] == ' '))
		|| (!ft_strncmp(cmd, "export", 6) && (!cmd[6] || cmd[6] == ' ')));
}
