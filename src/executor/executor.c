/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/10 14:21:58 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_executor(t_ms *ms)
{
	char	**args;

	args = ft_split(ms->input, ' ');
	if (!ft_strncmp(args[0], "cd", 2))
	{
		if (!args[1] || args[1][0] == ' ')
			ms_cd(ms, NULL);
		else if (args[1] && !args[2])
			ms_cd(ms, args[1]);
		else
			ms_error_handler(ms, "cd: invalid args", 0);
	}
	else if (!ft_strncmp(args[0], "env", 3))
		ms_env(ms);
	else if (!ft_strncmp(args[0], "pwd", 3))
		ms_pwd(ms);
	else if (!ft_strncmp(args[0], "unset", 5))
		ms_unset(ms, args);
	else if (!ft_strncmp(args[0], "exit", 4))
		ms_exit(ms, args);
	else if (!ft_strncmp(args[0], "echo", 4))
		ms_echo(ms, args);
	else if (!ft_strncmp(args[0], "export", 6))
		ms_export(ms, args);
	ft_free(args);
}
