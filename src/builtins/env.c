/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/23 16:02:45 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
ENV builtin command handler.
Prints the env variables with set up values inside the ms_env struct.
(Existing variables with no value are handled by EXPORT builtin)
*/
int	ms_env(t_ms *ms)
{
	t_list	*current;

	current = ms->ms_env;
	while (current)
	{
		if (ft_strchr((char *)current->content, '='))
			ft_printf("%s\n", (char *)current->content);
		current = current->next;
	}
	return (0);
}

/*
PWD builtin command handler.
Retrieves the CWD and prints it, with 3 possible cases:
	-If env is set and has a PWD entry, returns its value.
	-If this fails, tries to get the cwd by calling getcwd.
	-If no PWD/CWD can be retrieved, outputs a non-critical error.
*/
int	ms_pwd(t_ms *ms)
{
	char	*cwd;

	if (ms->filt_args[1])
		return (ms_error_handler(ms, "pwd: Too many arguments", 0), 1);
	if (ms_get_env_variable(ms, "PWD"))
	{
		ft_printf("%s\n", ms_get_env_variable(ms, "PWD"));
		return (0);
	}
	else
	{
		cwd = NULL;
		cwd = getcwd(cwd, PATH_MAX);
		if (cwd)
		{
			gc_add(cwd, &ms->gc);
			ft_printf("%s\n", cwd);
			return (0);
		}
	}
	ms_error_handler(ms, "pwd: Cannot retrieve current directory", 0);
	return (1);
}
